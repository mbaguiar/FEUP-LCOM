#include "serialport.h"

int hook_id_uart = 4;

static queue * recQueue;
static queue * sndQueue;
static int completeWord = 0;
static int emptyTHR = 0;

int serialport_subscribe(){
        unsigned long data;

        if (sys_irqsetpolicy(UART_COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_uart) != OK) {
                print_location();
                print_dbg("Error calling sys_irqsetpolicy function\n");
                return 1;
        } else if (sys_irqenable(&hook_id_uart) != OK) {
                print_location();
                print_dbg("Error calling sys_irqenable function\n");
                return 1;
        }

        if (serialport_set_LCR_config(UART_LCR_8BITS, UART_LCR_2STBIT, UART_LCR_0PAR, UART_RATE) != OK) {
                print_location();
                print_dbg("Error setting base LCR config\n");
                return 1;
        }

        // enable FIFO
        data = 0;
        data = UART_FCR_EF | UART_FCR_CRF | UART_FCR_CTF | UART_FCR_FITL;

        if (sys_outb(UART_COM1_ADDR + UART_W_FCR, data) != OK) {
                print_location();
                print_dbg("Error enabling FIFO\n");
                return 1;
        }

        //enable interrupts

        if (sys_inb(UART_COM1_ADDR + UART_RW_IER, &data) != OK) {
                print_location();
                print_dbg("Error getting IER config\n");
                return 1;
        }

        data |= UART_IER_ERDI | UART_IER_ETEI | UART_IER_ERLSI;

        if (sys_outb(UART_COM1_ADDR + UART_RW_IER, data) != OK) {
                print_location();
                print_dbg("Error writing new IER config\n");
                return 1;
        }

        recQueue = new_queue();
        sndQueue = new_queue();

        return 0;
}

int serialport_unsubscribe(){

        unsigned long data;

        if (sys_inb(UART_COM1_ADDR + UART_RW_IER, &data) != OK) {
                print_location();
                print_dbg("Error getting IER config\n");
                return 1;
        }

        data &= ~UART_IER_ERDI | ~UART_IER_ETEI | ~UART_IER_ERLSI;

        if (sys_outb(UART_COM1_ADDR + UART_RW_IER, data) != OK) {
                print_location();
                print_dbg("Error writing new IER config\n");
                return 1;
        }

        if (sys_irqdisable(&hook_id_uart) != OK) {
                print_location();
                print_dbg("Error calling sys_irqdisable\n");
                return 1;
        } else if(sys_irqrmpolicy(&hook_id_uart) != OK) {
                print_location();
                print_dbg("Error calling sys_irqrpolicy\n");
                return 1;
        }

        delete_queue(recQueue);
        delete_queue(sndQueue);

        return 0;

}

int serialport_set_LCR_config(unsigned long bits, unsigned long stop, long parity, unsigned long rate){
        unsigned long data;

        if (sys_inb(UART_COM1_ADDR + UART_RW_LCR, &data) != OK) {
                print_location();
                print_dbg("Error getting LCR config\n");
                return 1;
        }
        // clear all bits except "Set Break Enable"
        data &= UART_LCR_CLEAR;
        // set dlab to 1
        data |= UART_LCR_DLAB;
        // set word length
        data |= (bits & (UART_LCR_WLS0 | UART_LCR_WLS1));
        // set no. of stop bits
        if (stop == 2) data |= UART_LCR_NSB;
        // set parity
        data |= (parity << 3  & (UART_LCR_PAR0 | UART_LCR_PAR1 | UART_LCR_PAR2));

        //write new config
        if (sys_outb(UART_COM1_ADDR + UART_RW_LCR, data) != OK) {
                print_location();
                print_dbg("Error writing new LCR config\n");
                return 1;
        }
        // get divisor latch value based on rate
        unsigned long dlv = UART_BR_DIVIDEND/rate;
        unsigned long byte = dlv & BYTE_MASK;

        if (sys_outb(UART_COM1_ADDR + UART_RW_DLL, byte) != OK) {
                print_location();
                print_dbg("Error wrting LSB of DL\n");
                return 1;
        }

        byte = (dlv >> 8) & BYTE_MASK;

        if (sys_outb(UART_COM1_ADDR + UART_RW_DLM, byte) != OK) {
                print_location();
                print_dbg("Error wrting MSB of DL\n");
                return 1;
        }

        //set dlab to 0

        if (sys_inb(UART_COM1_ADDR + UART_RW_LCR, &data) != OK) {
                print_location();
                print_dbg("Error getting LCR config\n");
                return 1;
        }

        data &= ~UART_LCR_DLAB;

        if (sys_outb(UART_COM1_ADDR + UART_RW_LCR, data) != OK) {
                print_location();
                print_dbg("Error writing new LCR config\n");
                return 1;
        }

        return 0;

}

void serialport_int_handler(){
        unsigned long data;

        if (sys_inb(UART_COM1_ADDR + UART_R_IIR, &data) != OK) {
                print_location();
                print_dbg("Error reading IIR\n");
                return;
        }

        if (!(data & UART_IIR_IS)) {
                print_location();
                switch((data & (UART_IIR_IOMASK)) >> 1) {
                case UART_IIR_RLS:
                        serialport_handle_lsr();
                        break;
                case UART_IIR_CT: //character timeout is handled the same way as received data
                case UART_IIR_RDA:
                        if (data & (UART_IIR_FIFO_EN))
                                serialport_handle_recieved_data();
                        else {
                                print_location();
                                print_dbg("FIFO not enabled\n");
                                return;
                        }
                        break;
                case UART_IIR_TE:
                        emptyTHR = 1;
                        if (sndQueue->elem_count > 0) {
                                // test if FIFO is Enable
                                if (data & (UART_IIR_FIFO_EN))
                                        serialport_handle_transmitter_empty();
                                else {
                                        print_location();
                                        print_dbg("FIFO not enabled\n");
                                        return;
                                }
                        }
                        break;
                default:
                        print_location();
                        print_dbg("Error handling interrupt\n");
                        return;
                }
        }

}

void serialport_handle_recieved_data(){

        while(serialport_LSR_manager(RC_READY)) {
                unsigned long newChar;
                if (sys_inb(UART_COM1_ADDR + UART_R_RCR, &newChar) != OK) {
                        print_location();
                        print_dbg("Error reading from FIFO\n");
                        return;
                }
                print_dbg("Received %c\n", newChar);
                char * charToQueue = (char *) malloc(2*sizeof(char));
                *charToQueue = (char) newChar;
                if (*charToQueue == TERM_CHAR) completeWord++;
                push_queue(recQueue, charToQueue);
        }
}

void serialport_handle_transmitter_empty(){
        emptyTHR = 0;

        while(serialport_LSR_manager(THR_EMPTY) && sndQueue->elem_count > 0) {
                queue_elem * e = sndQueue->front;
                char * c = (char *)pop_queue(sndQueue);
                print_dbg("Pushing %c\n", *c);
                if (sys_outb(UART_COM1_ADDR + UART_W_THR, *c) != OK) {
                        print_location();
                        print_dbg("Error sending character\n");
                        return;
                }
                free(e);
        }
}

void serialport_handle_lsr(){

        while (serialport_LSR_manager(LSR_ERR)) {
                unsigned long garbage;
                if (sys_inb(UART_COM1_ADDR + UART_R_RCR, &garbage) != OK) {
                        print_location();
                        print_dbg("Error reading from FIFO\n");
                        return;
                }
        }
}

int serialport_LSR_manager(LSR_state lsr){
        unsigned long data;
        if (sys_inb(UART_COM1_ADDR + UART_R_LSR, &data) != OK) {
                print_location();
                print_dbg("Error reading LSR\n");
                return 0;
        }
        switch (lsr) {
        case RC_READY:
                return (data & UART_LSR_RR);
        case THR_EMPTY:
                return (data & UART_LSR_THRE);
        case LSR_ERR:
                return ((data & UART_LSR_RR) && (data & (UART_LSR_FE | UART_LSR_PE | UART_LSR_OE)));
        default:
                return 0;
        }
}


queue_elem * new_queue_elem(void * e){
        queue_elem * qe = malloc(sizeof(queue_elem));
        qe->elem = e;
        qe->next = NULL;
        return qe;
}


queue * new_queue(){
        queue * q = malloc(sizeof(queue));
        q->front = NULL;
        q->back = NULL;
        q->elem_count = 0;
        return q;
}

void push_queue(queue * q, void * elem){
        queue_elem * qe = new_queue_elem(elem);
        if (q->elem_count != 0) {
                q->back->next = qe;
                q->back = qe;
        } else {
                q->front = qe;
                q->back = qe;
        }
        q->elem_count++;
}

void * pop_queue(queue * q){
        if (q->elem_count == 0) return NULL;
        else {
                void * e = q->front->elem;
                q->front = q->front->next;
                q->elem_count--;
                return e;
        }
}

void delete_queue(queue * q){
        while(q->elem_count) {
                queue_elem * fe = q->front;
                char * e = pop_queue(q);
                free(e);
                free(fe);
        }
        free(q);
}

void push_string_queue(queue * q, char * string){
        size_t len = strlen(string);
        unsigned int i = 0;
        while(len > 0) {
                char * c = malloc(sizeof(char));
                *c = string[i];
                push_queue(q, c);
                i++;
                len--;
        }
        char * c = malloc(sizeof(char));
        *c = TERM_CHAR;
        push_queue(q, c);
}

char * return_fword_queue(queue *q){
        if (completeWord) {
                char * string = malloc(q->elem_count * 2 * sizeof(char));
                *string = '\0';
                while (q->elem_count) {
                        queue_elem * qe = q->front;
                        char * e = pop_queue(q);
                        if (*e != TERM_CHAR) {
                                e[1] = '\0';
                                strcat(string, e);
                                free(qe);
                        } else {
                                free(qe);
                                completeWord--;
                                return string;
                        }
                }
        }
        return NULL;
}

int checkWord(queue *q, char * str){
        if (completeWord) {
                char * string = malloc(q->elem_count * 2 * sizeof(char));
                *string = '\0';
                int i = 0;
                int r = 0;
                queue_elem *qe = q->front;
                while (1) {
                        char * e = (char *)qe->elem;
                        if (*e != TERM_CHAR) {
                                e[1] = '\0';
                                strcat(string, e);
                                i++;
                                qe = qe->next;
                        } else {
                                r = strcmp(string, str);
                                break;
                        }

                        if (r != 0) return 0;
                        free(string);
                        char * st = return_fword_queue(q);
                        if (strcmp(st, str)) {
                                free(st);
                                return 0;
                        }else {
                                free(st);
                                return 1;
                        }
                }
        }
        return 0;
}

void serialport_sendStringToBuffer(char * string){
        push_string_queue(sndQueue, string);
        if (serialport_LSR_manager(THR_EMPTY)) serialport_handle_transmitter_empty();
}

char * serialport_getWordFromBuffer(){
        return return_fword_queue(recQueue);
}
int serialport_emptyRecQueue(){
        return recQueue->elem_count == 0;
}
int serialport_checkForCancel(){
        return checkWord(recQueue, "cancel");
}

void serialport_cleanQueues() {
        delete_queue(recQueue);
        delete_queue(sndQueue);
        recQueue = new_queue();
        sndQueue = new_queue();
}
