#include "test4.h"
#include <minix/driver.h>

static state_t st = INIT; // initial state; keep state

int mouse_test_packet(unsigned short cnt){

	int r, ipc_status, irq_set = BIT(hook_id_mouse);
	message msg;
	unsigned long statusByte, garbage;

	//checks if value passed for counter is valid
	if (cnt <= 0) {
		printf("Invalid value for counter;\n");
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

	//cleans out buffer
	readStatusByte(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
	}

	while(cnt > 0) { /* You may want to use a different condition */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					if (mouse_handler() == -1){ //processes mouse interrupt
						display_packet(); //displays mouse data
						cnt--;
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

	//cleans outbuffer
	readStatusByte(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
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

	return 0;
}

int mouse_test_async(unsigned short idle_time){

	int r, ipc_status;
	int counter = 0;
	int irq_set_mouse = BIT(hook_id_mouse);
	int irq_set_timer = BIT(hook_id_timer);
	message msg;
	unsigned long statusByte, garbage;
	unsigned char timer0 = 0;

	//checks if value passed as time is valid
	if (idle_time <= 0) {
		printf("Invalid value for time\n");
		return 1;
	}

	//sets timer frequency to base frequency = 60
	if (timer_set_frequency(timer0, 60) != 0 ){
		printf("Error setting frequency of Timer 0 to %u.\n", 60);
		return 1;
	}

	//subscribes timer's interrupts
	if (timer_subscribe() < 0) {
		printf("Error subscribing timer's interrupts\n");
		return 1;
	}

	//subscribes mouse's interrupts
	if (mouse_subscribe()) {
		printf("Error subscribing mouses's interrupts\n");
		return 1;
	}

	//enables mouse data reporting
	if (enable_mouse()) {
		printf("Error enabling data report\n");
		return 1;
	}

	//cleans out buffer
	readStatusByte(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
	}

	while((counter/60.0) < idle_time) { /* You may want to use a different condition */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_mouse) { /* subscribed interrupt */
					if (mouse_handler() == -1){ //processes mouse interrupt
						display_packet(); //displays mouse data
					}
					counter = 0;
				}
				if (msg.NOTIFY_ARG & irq_set_timer)
					counter++; //increments counter for idle time
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	//cleans out buffer
	readStatusByte(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
	}

	//disables mouse data reporting
	if (disable_mouse()) {
		printf("Error disabling data report\n");
		return 1;
	}

	//unsubscribes mouse's interrupts
	if (mouse_unsubscribe()) {
		printf("Error unsubscribing mouse's interrupts\n");
		return 1;
	}

	//unsubscribes timer's interrupts
	if (timer_unsubscribe()) {
		printf("Error unsubscribing timer's interrupts\n");
		return 1;
	}

	return 0;

}

int mouse_test_remote(unsigned long period, unsigned short cnt){
	unsigned long kbc_cmd, garbage;

	//checks if value passed as counter is valid
	if (cnt <= 0) {
		printf("Invalid value for counter;\n");
		return 1;
	}

	//sends command to KBC asking to read command byte
	if (sendCmdToKBC(KBC_CMD_REG, 0x20) != 0) {
		printf("Error in sending command %x to KBC_CMD_REG\n", 0x20);
		return 1;
	}

	//reads KBC command byte
	if(sys_inb(OUT_BUF, &kbc_cmd) !=0){
		printf("Error reading command byte\n");
		return 1;
	}

	//sends command to KBC asking to write a new command byte
	if (sendCmdToKBC(KBC_CMD_REG, 0x60) != 0) {
		printf("Error in sending command %x to KBC_CMD_REG\n", 0x20);
		return 1;
	}

	//sends new command byte to disable mouse's interrupts
	if (sendCmdToKBC(0x60, (kbc_cmd & 0xFD)) != 0){
		printf("Error writing new command byte\n");
		return 1;
	}

	//disables mouse data reporting
	if (disable_mouse()){
		printf("Error disabling mouse\n");
		return 1;
	}

	if (enable_mouse()) {
		printf("Error enabling mouse\n");
		return 1;
	}

	if (disable_mouse()) {
		printf("Error enabling mouse\n");
		return 1;
	}

	//sends command to mouse to enable remote mode
	if (sendCmdToMouse(0xF0) != 0){
		printf("Error setting mode to remote\n");
		return 1;
	}

	//subscribes timer's interrupts
	if (timer_subscribe() < 0){
		printf("Error subscribing timer\n");
		return 1;
	}

	//cleans out buffer
	readStatusByte(&garbage);
	if (garbage & OBF){
		sys_inb(OUT_BUF,&garbage);
	}

    period *= 1000;

	while (cnt > 0){
        
        if (sendCmdToMouse(0xEB) != 0){
            printf("Error sending read data command\n");
            return 1;
        }

        while (1){
            if (mouse_handler() == -1) { //gets mouse data packet
                display_packet(); //displays mouse data packet
                cnt--;
                break;
            }
        }

        tickdelay(micros_to_ticks(period));


    }

	//unsubscribes timer's interrupts
	if (timer_unsubscribe() != 0){
		printf("Error unsubscribing timer\n");
		return 1;
	}

	//sends command to mouse to set to stream mode
	if (sendCmdToMouse(0xEA) != 0){
		printf("Error setting mode to stream mode\n");
		return 1;
	}

	//sends a command byte to KBC asking to write a new command byte
	if (sendCmdToKBC(KBC_CMD_REG, 0x60) != 0){
		printf("Error sending command to KBC\n");
		return 1;
	}

	//sends new command byte to enable mouse's interrupts
	if (sendCmdToKBC(0x60, kbc_cmd | BIT(1)) != 0){
		printf("Error re-enabling interrupts\n");
		return 1;
	}

	return 0;

}

//handle events
void check_hor_line(event_t * evt) {

	switch (st) {
    case INIT:
        //start drawing
		if(evt->type == RDOW )
			st = DRAW;
		break;
	case DRAW:
		if(evt->type == MOVE ) {
            //if sign is different then the movement has a negative slope
			if (evt->x * evt->y <= 0){
				st = INIT;
			} else {
                // compare x movement with length for both positive and negative values
				if ( ((evt->x >= evt->length) && (evt->length > 0)) || ((evt->x <= evt->length) && (evt->length < 0)) ) st = COMP;
			}
            //end drawing because right button is not pressed
		} else if(evt->type == RUP )
			st = INIT;
		break;
	default:
		break; }

}


int mouse_test_gesture(short length){

	static event_t evt;
	int r, ipc_status, irq_set = BIT(hook_id_mouse);
	message msg;
	unsigned long statusByte, garbage;

	if (length == 0){
		printf("Length can't be null\n");
		return 1;
	}

	evt.length = length;

	if (mouse_subscribe() != 0) {
		printf("Error subscribing mouse interrupts\n");
		return 1;
	}

	if (enable_mouse() != 0) {
		printf("Error enabling data report\n");
		return 1;
	}

	readStatusByte(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
	}

	while(st != COMP) { /* You may want to use a different condition */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					if (mouse_handler() == -1) {
                        display_packet();
                    
                        if (((packet[0] & RB) >> 1) && st == INIT) evt.type = RDOW; //was not drawing
                        
                        else if (((packet[0] & RB) >> 1) && st == DRAW) evt.type = MOVE; //was drawing and RB is pressed
                        
                        else if (!((packet[0] & RB) >> 1)) evt.type = RUP; //RB is not pressed
                        
                        //get x and y deltas
						xyval XY = getXY();
						evt.x = XY.x;
                        evt.y = XY.y;
                        //handle event
						check_hor_line(&evt);
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

	readStatusByte(&statusByte);
	if (statusByte & OBF){
		sys_inb(OUT_BUF,&garbage);
	}

	if (disable_mouse() != 0) {

		printf("Error disabling mouse\n");
		return 1;
	}

	if (mouse_unsubscribe() != 0) {

		printf("Error unsubscribing mouse\n");
		return 1;
	}

	return 0;

}

void display_packet() {

	printf("B1=0x%02x ", packet[0]);
	printf("B2=0x%02x ", packet[1]);
	printf("B3=0x%02x ", packet[2]);

	printf("LB=%u ", (packet[0] & LB)); //if left button is pressed
	printf("MB=%u ", (packet[0] & MB) >> 2); //if middle button is pressed
	printf("RB=%u ", (packet[0] & RB) >> 1); //if right button is pressed
	printf("XOV=%u ", (packet[0] & XOV) >> 6); //if there was overload in x value
	printf("YOV=%u ", (packet[0] & YOV) >> 5); //if there was overload in the y value

	xyval XY = getXY(); //gets values for x and y

	printf("X=%d ", XY.x);
	printf("Y=%d\n\n", XY.y);

}
