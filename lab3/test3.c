#include "test3.h"
#include <minix/driver.h>

int kbd_test_scan(unsigned short ass) {
	cntr = 0;
	int r, ipc_status, irq_set = BIT(hook_id_kbd), doublebyte = 0;
	message msg;
	unsigned long scancode = 0;

	//subscribes keyboard's interrupts
	if (keyboard_subscribe() != 0) {
		printf("Error subscribing keyboard interrupts\n");
		return 1;
    }
    
	while( scancode != ESC_KEY ) { /* You may want to use a different condition */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */

					if (ass == 0) { //reads outbuffer with functions in c

						if (!readOutBuf(&scancode)) {

							printScancode(&scancode, &doublebyte);
						}
					} else if (ass == 1) { //reads outbuffer with functions in assembly
                        
                        if(readOutBufASM() != 0){
                            return 1;
                        } else scancode = scancodeASM;

                        printScancode(&scancode, &doublebyte);
                    }

				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	//unsubscribes keyboard's interrupts
	if(keyboard_unsubscribe() != 0) {
		printf("Error unsubscribing keyboard interrupts\n");
		return 1;
	}

#ifdef LAB3
	if (ass == 0) printf("Number of sys_inb calls: %d\n", cntr);
#endif

	return 0;
}

int kbd_test_poll() {
	cntr = 0;
	unsigned long scancode = 0;
	int doublebyte = 0;
	unsigned long kbc_cmd;

	//sends command to kbc command register asking to read command byte
	if (sendCmdToKBC(KBC_CMD_REG, 0x20) != 0) {
		printf("Error in sending command %x to KBC_CMD_REG\n", 0x20);
		return 1;
	}

	//reads command byte
	if (readOutBuf(&kbc_cmd) != 0) {
		printf("Error reading command byte\n");
		return 1;
	}

	while (scancode != ESC_KEY){

		if (readOutBuf(&scancode) != 0){
			return 1;
		}

		printScancode(&scancode, &doublebyte);

		tickdelay(micros_to_ticks(DELAY_US));

	}
#ifdef LAB3
	printf("Number of sys_inb calls: %d\n", cntr);
#endif

	kbc_cmd = kbc_cmd | BIT(0); //changes command byte to enable interrupts again

	//sends command to kbc command register asking to write a new kbc command byte
	if (sendCmdToKBC(KBC_CMD_REG, 0x60) != 0) {
		printf("Error sending command %x to KBC_CMD_REG\n", 0x60);
		return 1;
	}
	//writes new kbc command byte
    if(sendCmdToKBC(IN_BUF, kbc_cmd) != 0) {
    		printf("Error enabling interrupts again\n");
    		return 1;
    }
    
    return 0;
}

int kbd_test_timed_scan(unsigned short n) {

    int counter = 0;
	int r, ipc_status, irq_set_keyboard = BIT(hook_id_kbd), irq_set_timer = BIT(hook_id_timer), doublebyte = 0;
	message msg;
    unsigned long scancode = 0;
    unsigned char timer0 = 0;
    
    //checks if time is a valid value
    if (n < 1){
        printf("Invalid time\n");
        return 1;
    }

    //sets timer frequency to base frequency = 60
    if (timer_set_frequency(timer0, 60) != 0 ){
        printf("Error setting frequency of Timer 0 to %u.\n", 60);
        return 1;
    }

    //subscribes keyboard's interrupts
    if (keyboard_subscribe() != 0) {
        printf("Error subscribing keyboard interrupts\n");
		return 1;
    }
    
    //subscribes timer's interrupts
    if (timer_subscribe() < 0) {
        printf("Error subscribing Timer0 interrupts\n");
        return 1;
    }

    while( scancode != ESC_KEY && ((counter/60.0) < n)) { /* You may want to use a different condition */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_keyboard) { /* subscribed interrupt */

					counter = 0;

						if (!readOutBuf(&scancode)) {
                            
							printScancode(&scancode, &doublebyte);
                } 
            }
                if (msg.NOTIFY_ARG & irq_set_timer) {
					counter++;					
                }
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
    }

    //unsubscribes timer's interrupts
    if (timer_unsubscribe() != 0){
        printf("Error unsubscribing Timer0 interrupts\n");
        return 1;
    }

    	//unsubscribes keyboard's interrupts
    if (keyboard_unsubscribe() != 0){
        printf("Error unsubscribing keyboard interrupts\n");
        return 1;
    }

    return 0;
}

void printScancode(unsigned long *scancode, int * doublebyte){
    if (*scancode == DOUBLEBYTE) { //checks if scancode is a double byte
        *doublebyte = 1;

    }else if (*doublebyte) {

        if(*scancode & BREAKCODE) {
            printf("Breakcode: 0x%02x 0x%02x\n", DOUBLEBYTE, *scancode);
        } else
            printf("Makecode: 0x%02x 0x%02x\n", DOUBLEBYTE, *scancode);

        *doublebyte = 0;

    }else if (*scancode & BREAKCODE) {
        printf("Breakcode: 0x%02x\n", *scancode);

    } else
        printf("Makecode: 0x%02x\n", *scancode);
}
