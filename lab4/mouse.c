#include "mouse.h"

int hook_id_mouse = 12;

unsigned long packet[3];

int mouse_subscribe() {

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) != OK) {
		print_location();
		print_dbg("Error calling sys_irqsetpolicy function\n");
		return 1;
	} else if (sys_irqenable(&hook_id_mouse) != OK) {
		print_location();
		print_dbg("Error calling sys_irqenable function\n");
		return 1;
	}

	return 0;
}

int mouse_unsubscribe() {

	if (sys_irqdisable(&hook_id_mouse) != OK) {
		print_location();
		print_dbg("Error calling sys_irqdisable\n");
		return 1;
	} else if(sys_irqrmpolicy(&hook_id_mouse) != OK) {
		print_location();
		print_dbg("Error calling sys_irqrpolicy\n");
		return 1;
	}

	return 0;
}

int enable_mouse() {

	int error = 0;

	while (sendCmdToMouse(ENABLE_MOUSE) != OK) {
		error++;
		if (error == 5) {
			print_location();
			print_dbg("Error enabling mouse\n");
			return 1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

	return 0;
}

int disable_mouse() {

	int error = 0;

	while (sendCmdToMouse(DISABLE_MOUSE) != OK) {
		error++;
		if (error == 5) {
			print_location();
			print_dbg("Error disabling mouse\n");
			return 1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

	return 0;

}

int sendCmdToMouse(unsigned long cmd) {

	int repeat = 1;
	int error = 0;
	unsigned long data;

	while (repeat == 1 && error < 5) {
		if (sys_outb(STAT_REG, WRITE_MOUSE) != OK) { //tells the mouse a cmd will be written
			print_location();
			print_dbg("Error sending command to KBC\n");
			return 1;
		}
		if (sys_outb(OUT_BUF, cmd) != OK) {
			print_location();
			print_dbg("Error sending command outbuffer\n");
			return 1;
		}
		tickdelay(micros_to_ticks(DELAY_US));

		if (sys_inb(OUT_BUF, &data) != OK) {
			print_location();
			print_dbg("Error reading outbuffer\n");
			return 1;
		}
		if (data != ERROR && data != RESEND) //checks if no error has ocurred while sending the cmd to the mouse
			repeat = 0;
		else error++;

		tickdelay(micros_to_ticks(DELAY_US));

		if (error == 5) {
			print_location();
			print_dbg("Error timeout\n");
			return 1;
		}
	}
	return 0;
}

int mouse_handler() {

	static int pcktCounter = 0;
	unsigned long data;

	if (readOutBuf(&data) != OK)
		return 1;

	if (pcktCounter == 0) {

		if (data & BIT(3)) {

			packet[0] = data;
			pcktCounter++;
			return 0;
		}

	}else if (pcktCounter == 1) {

		packet[1] = data;
		pcktCounter++;
		return 0;

	}else if(pcktCounter == 2) {

		packet[2] = data;
		pcktCounter = 0;
		return -1;
	}

	return 0;

}

xyval getXY(){

	signed long x, y;

	if (packet[0] & XSIGN) { //checks if x is positive or negative
		x = packet[1];
		x |= (-1 << 8);

	} else x = packet[1];

	if (packet[0] & YSIGN) { //checks if y is positive or negative
		y = packet[2];
		y |= (-1 << 8);


	} else y = packet[2];

	xyval ret;
	ret.x = x;
	ret.y = y;

	return ret;

}
