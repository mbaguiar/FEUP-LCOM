#ifndef __SERIALPORT_H
#define __SERIALPORT_H

#include <minix/drivers.h>
#include "i8042.h"
#include "i8254.h"
#include "debug.h"

/** @defgroup SerialPort SerialPort
 * @{
 */

#define BYTE_MASK         0xFF

//UART

#define UART_COM1_ADDR    0x3F8
#define UART_COM2_ADDR    0x2F8
#define UART_COM1_IRQ     4
#define UART_COM2_IRQ     3
#define UART_COM1_VEC     0x0C
#define UART_COM2_VEC     0x0B



// R-READ W-WRITE RW-READ/WRITE

#define UART_R_RCR        0
#define UART_W_THR        0
#define UART_RW_IER       1
#define UART_R_IIR        2
#define UART_W_FCR        2
#define UART_RW_LCR       3
#define UART_R_LSR        5
#define UART_RW_DLL       0
#define UART_RW_DLM       1

// LCR REG

#define UART_LCR_WLS0     BIT(0)
#define UART_LCR_WLS1     BIT(1)
#define UART_LCR_NSB      BIT(2)
#define UART_LCR_PAR0     BIT(3)
#define UART_LCR_PAR1     BIT(4)
#define UART_LCR_PAR2     BIT(5)
#define UART_LCR_SBE      BIT(6)
#define UART_LCR_DLAB     BIT(7)
#define UART_LCR_CLEAR    0x40

#define UART_LCR_8BITS    3
#define UART_LCR_2STBIT   2
#define UART_LCR_0PAR     0
typedef enum {RC_READY, THR_EMPTY, LSR_ERR} LSR_state;

// LSR REG

#define UART_LSR_RR       BIT(0)
#define UART_LSR_OE       BIT(1)
#define UART_LSR_PE       BIT(2)
#define UART_LSR_FE       BIT(3)
#define UART_LSR_BI       BIT(4)
#define UART_LSR_THRE     BIT(5)
#define UART_LSR_TE       BIT(6)

// IER REG

#define UART_IER_ERDI     BIT(0)
#define UART_IER_ETEI     BIT(1)
#define UART_IER_ERLSI    BIT(2)
#define UART_IER_EMSI     BIT(3)

// IIR REG

#define UART_IIR_IS       BIT(0)
#define UART_IIR_IO       BIT(1)
#define UART_IIR_FIFO     BIT(5)
#define UART_IIR_FS       BIT(6)
#define UART_IIR_RDA      BIT(1)
#define UART_IIR_FIFO_EN  BIT(6)| BIT(7)
#define UART_IIR_TE       BIT(0)
#define UART_IIR_CT       BIT(2) | BIT(1)
#define UART_IIR_RLS      BIT(1) | BIT(0)
#define UART_IIR_IOMASK   BIT(1) | BIT(2) | BIT(3)

// FCR REG

#define UART_FCR_EF       BIT(0)
#define UART_FCR_CRF      BIT(1)
#define UART_FCR_CTF      BIT(2)
#define UART_FCR_DMA      BIT(3)
#define UART_FCR_64FE     BIT(5)
#define UART_FCR_FITL     BIT(6)

#define UART_RATE          19200
#define UART_BR_DIVIDEND  115200
#define TERM_CHAR         '$'

// QUEUE IMPLEMENTATION

typedef struct queue_elem queue_elem;

struct queue_elem{
  void * elem;
  queue_elem * next;
};

typedef struct {
  queue_elem * front;
  queue_elem * back;
  unsigned int elem_count;
} queue;

/**
 * @brief creates new queue element
 * @param  e data to put in element created
 * @return returns pointer to new queue element
 */
queue_elem * new_queue_elem(void * e);
/**
 * @brief creates new queue
 * @return returns pointer to new queue
 */
queue * new_queue();
/**
 * @brief pushes element passed as argument to queue q
 * @param q    queue to which elem is pushed
 * @param elem elem to be pushed to queue q
 */
void push_queue(queue * q, void * elem);
/**
 * @brief pops front element from queue passed as argument
 * @param  q queue
 * @return returns element that was poped from queue
 */
void * pop_queue(queue * q);
/**
 * @brief deletes queue passed as argument
 * @param q queue to be deleted
 */
void delete_queue(queue * q);
/**
 * @brief pushes string passed as argument to queue q, pushes each character to a different position
 * @param q      queue q
 * @param string string that is pushed to queue
 */
void push_string_queue(queue * q, char * string);
/**
 * @brief returns full front word from queue passed as argument
 * @param  q queue q
 * @return full string
 */
char * return_fword_queue(queue * q);
/**
 * @brief checks if there is a message in queue passed as argument
 * @param  q   queue q
 * @param  str message to be checked
 * @return returns 1 if string is found and returns 0 otherwise
 */
int checkWord(queue *q, char * str);


// SERIAL PORT FUNCTIONS

extern int hook_id_uart;

/**
 * @brief subscribe UART's interrupts
 * @return 0 upon success or non-zero otherwise
 */
int serialport_subscribe();

/**
 * @brief unsubscrives UART's interrupts
 * @return 0 upon success or non-zero otherwise
 */

int serialport_unsubscribe();

/**
 * @brief sets UART's LCR configuration and changes bitrate
 * @param bits  Word length
 * @param stop  Number of stop bits
 * @param parity Parity selection
 * @param rate Bitrate value
 * @return 0 upon success or non-zero otherwise
 */
int serialport_set_LCR_config(unsigned long bits, unsigned long stop, long parity, unsigned long rate);

/**
 * @brief handles UART's interrupts
 * @return
 */
void serialport_int_handler();

/**
 * @brief reads from received buffer and pushes characters into received queue
 * @return
 */
void serialport_handle_recieved_data();

/**
 * @brief pushes characters from send queue into transmitter holding buffer if possible
 * @return
 */

void serialport_handle_transmitter_empty();
/**
 * @brief handles lsr errors and discards corrupted chracters
 * @return
 */
void serialport_handle_lsr();

/**
 * @brief tests the status of the LSR
 * @param lsr state to be tested (receiver ready, transmitter empty or error)
 * @return return 1 if requested state is compatible with LSR status or 0 otherwise
 */
int serialport_LSR_manager(LSR_state lsr);

/**
 * @brief pushes a string to the send queue and, if possible, to the UART transmitter holding register
 * @param string string to be sent
 * @return
 */
void serialport_sendStringToBuffer(char * string);

/**
 * @brief returns a complete word from the received queue
 * @return return the first complete word in the received queue or NULL if no complete word is found
 */
char * serialport_getWordFromBuffer();

/**
 * @brief tests if received queue is empty
 * @return return 1 if recevied queue is empty or 0 otherwise
 */
int serialport_emptyRecQueue();

/**
 * @brief clears recevied and send queue
 * @return
 */
void serialport_cleanQueues();
/**
 * @brief
 * @return [description]
 */
int serialport_checkForCancel();

/**@}*/

#endif /* end of include guard: __SERIALPORT_H */
