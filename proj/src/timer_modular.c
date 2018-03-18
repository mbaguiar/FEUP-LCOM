#include "timer_modular.h"

int hook_id_timer = 0;

void (*timerListener)();

int timer_subscribe() {

	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer) != OK) {
		print_location();
		print_dbg("Error calling sys_irqsetpolicy function\n");
		return -1;
	} else if(sys_irqenable(&hook_id_timer) != OK) {
		print_location();
		print_dbg("Error calling sys_irqenable function\n");
		return -1;
	}

	return BIT(hook_id_timer);
}

int timer_unsubscribe() {

	if(sys_irqdisable(&hook_id_timer) != OK) {
		print_location();
		print_dbg("Error calling sys_irqdisble\n");
		return 1;
	} else if(sys_irqrmpolicy(&hook_id_timer) != OK) {
		print_location();
		print_dbg("Error calling sys_irqrmpolicy\n");
		return 1;
	}
	return 0;
}

void setTimerListener(void (*listener)()) {
	timerListener = listener;
}

void timer_handler() {
	timerListener();
}
