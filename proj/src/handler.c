#include "handler.h"
#include <minix/driver.h>

int handler_loop(){

								int r, ipc_status, irq_set_mouse = BIT(hook_id_mouse);
								int irq_set_kbd = BIT(hook_id_kbd);
								int irq_set_timer = BIT(hook_id_timer);
								int irq_set_rtc = BIT(hook_id_rtc);
								int irq_set_serialport = BIT(hook_id_uart);
								message msg;

								if (timer_subscribe() == -1) {
																printf("Error subscribing timer interrupts.\n");
																return 1;
								}

								if (keyboard_subscribe() != 0) {
																printf("Error subscribing keyboard interrupts.\n");
																return 1;
								}
								//subscribes mouse's interrupts
								if (mouse_subscribe() != 0) {
																printf("Error subscribing mouse interrupts\n");
																return 1;
								}

								//enables mouse data reporting
								if (enable_mouse() != 0) {
																printf("Error enabling data report\n");
																return 1;
								}

								if (rtc_subscribe(0, 0, 1) != 0) {
																printf("Error subscribing rtc interrupts\n");
																return 1;
								}

								if(serialport_subscribe() != 0) {
																printf("Error subscribing serial port\n");
								}

								while(1) { /* You may want to use a different condition */
																/* Get a request message. */
																if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
																								printf("driver_receive failed with: %d", r);
																								continue;
																}
																if (is_ipc_notify(ipc_status)) { /* received notification */
																								switch (_ENDPOINT_P(msg.m_source)) {
																								case HARDWARE: /* hardware interrupt notification */
																																if (msg.NOTIFY_ARG & irq_set_timer) {
																																								timer_handler();
																																}
																																if (msg.NOTIFY_ARG & irq_set_mouse) { /* subscribed interrupt */
																																								mouse_handler(); //processes mouse interrupt
																																}
																																if (msg.NOTIFY_ARG & irq_set_kbd) {
																																								keyboard_handler();
																																}
																																if (msg.NOTIFY_ARG & irq_set_rtc) {
																																								rtc_handler();
																																}
																																if (msg.NOTIFY_ARG & irq_set_serialport) {
																																								printf("sp int caught\n");
																																								serialport_int_handler();
																																}
																																break;
																								default:
																																break; /* no other notifications expected: do nothing */
																								}
																} else { /* received a standard message, not a notification */
																								 /* no standard messages expected: do nothing */
																}
								}

}

int unsubscribe() {

								unsigned long statusByte, garbage;
								//cleans outbuffer
								readStatusByte(&statusByte);
								if (statusByte & OBF) {
																sys_inb(OUT_BUF,&garbage);
								}

								if (rtc_unsubscribe() != 0) {
																printf ("Error unsubscribing rtc\n");
																return 1;
								}

								//disables mouse data reporting
								if (disable_mouse() != 0) {

																printf("Error disabling mouse\n");
																return 1;
								}

								//unsubscribes mouse's interrupts
								if (mouse_unsubscribe() != 0) {

																printf("Error unsubscribing mouse\n");
																return 1;
								}

								if (keyboard_unsubscribe() != 0) {

																printf("Error unsubscribing keyboard\n");
																return 1;
								}

								if (timer_unsubscribe() != 0) {
																printf("Error unsubscribing timer\n");
																return 1;
								}

								if (serialport_unsubscribe() != 0) {
																printf("Error unsubscribing serial port\n");
																return 1;
								}

								return 0;
}
