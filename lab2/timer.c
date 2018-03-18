#include <minix/syslib.h>
#include <minix/drivers.h>
#include "timer.h"
#include "i8254.h"

int hook_id = 0;
int counter = 0;

int timer_set_frequency(unsigned char timer, unsigned long freq) {

	// TIMER_FREQ/freq must be written in 16 bits, so freq can't be lower than 19

	if (freq < 19){
		printf("Invalid frequency.\n");
		return 1;
	}

	if(test_valid_timer(timer)) return 1;

	unsigned char config;

	if(timer_get_conf(timer, &config)) return 1;

	unsigned long ctrl_word = (BIT(3) | BIT(2) | BIT(1) | BIT(0)) & config; // Bitwise AND preserving only the 4 lsb's.

	// Assigning corresponding bit according to timer
	if (timer != 0){

		unsigned long n = timer + 5;
		ctrl_word = ctrl_word | BIT(n);

	}

	ctrl_word = ctrl_word | TIMER_LSB_MSB; //Building control word

	if(sys_outb(TIMER_CTRL, ctrl_word)){

		printf("Error writing control word 0x%x to the control register.\n", ctrl_word);
		return 1;

	}

	unsigned long new_frequency = TIMER_FREQ / freq;
	unsigned char lsb = (unsigned char) new_frequency; //Truncate last 8 bits
	unsigned char msb = (unsigned char ) (new_frequency >> 8); //Shift and truncate first 8 bits
	unsigned long cTimer = get_timer_address(timer);

	if (sys_outb(cTimer, lsb)){

		printf("Error loading timer %u with LSB for frequency configuration.\n");
		return 1;

	}

	if (sys_outb(cTimer, msb)){

		printf("Error loading timer %u with MSB for frequency configuration.\n");
		return 1;

	}

	return 0;

}

int timer_subscribe_int(void ) {

	int hook = hook_id;

	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK) {
		printf("Error calling sys_irqsetpolicy function\n");
		return -1;
	} else if(sys_irqenable(&hook_id) != OK) {
		printf("Error calling sys_irqenable function\n");
		return -1;
	}

	return BIT(hook);
}

int timer_unsubscribe_int() {

	if(sys_irqdisable(&hook_id) != OK) {
		printf("Error calling sys_irqdisble\n");
		return 1;
	} else if(sys_irqrmpolicy(&hook_id) != OK) {
		printf("Error calling sys_irqrmpolicy\n");
		return 1;
	}

	return 0;
}

void timer_int_handler() {

	counter++;

}

int timer_get_conf(unsigned char timer, unsigned char *st) {

	if(test_valid_timer(timer))
		return 1;

	unsigned long cmd = TIMER_RB_CMD | TIMER_RB_SEL(timer);

	if (sys_outb(TIMER_CTRL, cmd)){
		printf("Error writing read-back command to the control register.\n");
		return 1;
	}

	unsigned long config;
	unsigned long cTimer; //Selected timer address

	cTimer = get_timer_address(timer);

	if (sys_inb(cTimer, &config)){
		printf("Error reading configuration from timer %u.\n", timer);
		return 1;
	} else {

		*st = (unsigned char) config;
		return 0;
	}

}

int timer_display_conf(unsigned char conf) {

	printf("Timer Status: 0x%x\n", conf);

	unsigned int output = conf & OUTPUT;

	if(output) {
		printf("Out Pin is 1\n");
	} else printf("Out Pin is 0\n");

	unsigned int nullCount = conf & NULL_COUNT;

	if(nullCount) {
		printf("Null Count (1)\n");
	} else printf("Count available for reading (0)\n");

	unsigned int typeAc = conf & TIMER_LSB_MSB;

	printf("Type of access:");

	switch(typeAc) {
	case TIMER_LSB:
		printf("Read/Write least significant byte only\n");
		break;
	case TIMER_MSB:
		printf("Read/Write most significant byte only\n");
		break;
	case TIMER_LSB_MSB:
		printf("Read/Write least significant byte first, then most significant byte\n");
		break;
	case TIMER_CLC:
		printf("Counter Latch Command\n");
		break;
	}

	unsigned int mode = conf & MODE;

	printf("Operating mode: ");

	switch(mode) {
	case TIMER_INT_TC:
		printf("Interrupt on Terminal Count (Mode 0)\n");
		break;
	case TIMER_HW_RTOS:
		printf("Hardware Retriggerable One-shot (Mode 1)\n");
		break;
	case TIMER_RATE_GEN:
		printf("Rate Generator (Mode 2)\n");
		break;
	case TIMER_RATE_GEN_x:
		printf("Rate Generator (Mode 2)\n");
		break;
	case TIMER_SQR_WAVE:
		printf("Square Wave (Mode 3)\n");
		break;
	case TIMER_SQR_WAVE_x:
		printf("Square Wave (Mode 3)\n");
		break;
	case TIMER_SW_TRGSTR:
		printf("Software Triggered Strobe (Mode 4)\n");
		break;
	case TIMER_SW_TRGSTR_R:
		printf("Software Triggered Strobe Rettriggerable (Mode 5)\n");
		break;
	}

	unsigned int bcd = conf & TIMER_BCD;

	if(bcd == TIMER_BCD) {
		printf("Counting mode: BCD (1)\n");
	} else if(bcd == TIMER_BIN)
		printf("Counting mode: Binary (0)\n");

	return 0;
}

int timer_test_time_base(unsigned long freq) {
	unsigned char timer0 = 0;

	if(timer_set_frequency(timer0, freq)){
		printf("Error setting frequency of Timer 0 to %u.\n", freq);
		return 1;
	}

	return 0;
}

int timer_test_int(unsigned long time) {

	int r;
	int ipc_status;
	message msg;
	int irq_set = BIT(hook_id);
	const unsigned long timer_frequency = 60;

	if(time < 1) {
		printf("Invalid time value\n");
		return 1;
	}

	if(timer_test_time_base(timer_frequency)){
		printf("Error setting timer to base frequency\n");
		return 1;
	}

	if(timer_subscribe_int() < 0) {
		printf("Error subscribing Timer 0 interrupts\n");
		return 1;
	}

	do { /* You may want to use a different condition */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					timer_int_handler();
					if((counter % timer_frequency) == 0) {
						if (counter/timer_frequency == 1) printf("1 second has passed\n");
						else printf("%d seconds have passed\n", counter/timer_frequency);
						time--;
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	} while(time);

	if(timer_unsubscribe_int()) {
		printf("Error unsubscribing Timer 0 interrupts\n");
		return 1;
	}

	return 0;
}

int timer_test_config(unsigned char timer) {
	unsigned char config;
	if(timer_get_conf(timer, &config)) {
		printf("Error getting configuration.\n");
		return 1;
	}
	timer_display_conf(config);
	return 0;
}

unsigned long get_timer_address(unsigned char timer){

	if (timer == 0){
		return TIMER_0;
	} else if (timer == 1){
		return TIMER_1;
	} else if (timer == 2){
		return TIMER_2;
	}

}

int test_valid_timer(unsigned char timer){

	if (timer == 0 || timer == 1 || timer == 2) return 0;
	else {
		printf("Timer %u does not exist.\n", timer);
		return 1;
	}

}
