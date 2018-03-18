#include "keyboard.h"

int hook_id_kbd = 1;

int keyboard_subscribe() {

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd) != OK) {
		print_location();
		print_dbg("Error calling sys_irqsetpolicy function\n");
		return 1;
	} else if (sys_irqenable(&hook_id_kbd) != OK) {
		print_location();
		print_dbg("Error calling sys_irqenable function\n");
		return 1;
	}

	return 0;
}

int keyboard_unsubscribe() {

	if (sys_irqdisable(&hook_id_kbd) != OK) {
		print_location();
		print_dbg("Error calling sys_irqdisable\n");
		return 1;
	} else if(sys_irqrmpolicy(&hook_id_kbd) != OK) {
		print_location();
		print_dbg("Error calling sys_irqrpolicy\n");
		return 1;
	}

	return 0;
}

int readStatusByte(unsigned long *statusbyte) {

#ifdef LAB3
	cntr++;
#endif

	if(sys_inb(STAT_REG, statusbyte) != OK) {
		print_location();
		print_dbg("Error reading status byte\n");
		return 1;
	}

	return 0;
}

int readOutBuf(unsigned long *data){
	unsigned long statusbyte;

	while(1){

		if (readStatusByte(&statusbyte) != OK) return 1;

		if (((statusbyte & OBF) == 1) && ((statusbyte & AUX) == 0)) { //checks if there's data on the out buffer and if that data isn not mouse that

			if ((statusbyte & (PAR_ERR | TO_ERR)) != 0) { //checks if data on the out buffer is valid
				print_location();
				print_dbg("Invalid data on buffer\n");
				return 1;
			}

#ifdef LAB3
			cntr++;
#endif

			if(sys_inb(OUT_BUF, data) != OK) {
				print_location();
				print_dbg("Error reading output from buffer\n");
				return 1;
			} else break;

		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
	return 0;
}

int sendCmdToKBC(unsigned long reg, unsigned long cmd) {

	unsigned long stat;

	while(1) {

		while (1){
			if (readStatusByte(&stat) != 0) return 1;
			//loop while 8042 input buffer is not empty
			if ((stat & IBF) == 0) { //checks if input buffer is empty
				break;
			} else {
				print_location();
				print_dbg("Input buffer full \n");
			}
			tickdelay(micros_to_ticks(DELAY_US));
		}

		if (sys_outb(reg, cmd) == 0) { //no args command
			return 0;
		}

		tickdelay(micros_to_ticks(DELAY_US));
	}

}




