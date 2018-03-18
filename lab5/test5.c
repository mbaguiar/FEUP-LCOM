#include "test5.h"
#include "video_test.h"

int video_test_init(unsigned short mode, unsigned short delay) {

    if (vg_init(mode) == NULL){
        printf("vg_init() error\n");
        return 1;
    }

  sleep(delay);

  if(vg_exit() != 0) {

		printf("Error exiting video graphics mode.\n");
		return 1;
	}
	return 0;
}


int video_test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {

	int r, ipc_status, irq_set = BIT(hook_id_kbd);
	message msg;
	unsigned long scancode = 0;

	if (vg_init(MODE105) == NULL){
        printf("vg_init() error\n");
        return 1;
    }

	if (keyboard_subscribe() != 0) {
		printf("Error subscribing keyboard's interrupts\n");
		return 1;
	}

	if (vg_square(x, y, size, color) != 0) {
		printf("Error printing square.\n");
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

				readOutBuf(&scancode);

				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (keyboard_unsubscribe() != 0) {
		printf("Error unsubscribing keyboard's interrupts\n");
		return 1;
	}

	if(vg_exit() != 0) {

		printf("Error exiting video graphics mode\n");
		return 1;
	}
	return 0;

}

int video_test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {

	int r, ipc_status, irq_set = BIT(hook_id_kbd);
	message msg;
	unsigned long scancode = 0;

	if (vg_init(MODE105) == NULL){
        printf("vg_init() error\n");
        return 1;
    }


	if (keyboard_subscribe() != 0) {
		printf("Error subscribing keyboard's interrupts\n");
		return 1;
	}

	if (vg_line(xi, yi, xf, yf, color) != 0){
		printf("Error printing line.\n");
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

				readOutBuf(&scancode);

				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (keyboard_unsubscribe() != 0) {
		printf("Error unsubscribing keyboard's interrupts\n");
		return 1;
	}

	if(vg_exit() != 0) {

		printf("Error exiting video graphics mode\n");
		return 1;
	}
	return 0;
}

int test_xpm(char *xpm[], unsigned short xi, unsigned short yi){

	int r, ipc_status, irq_set = BIT(hook_id_kbd);
	message msg;
	unsigned long scancode = 0;

	if (vg_init(MODE105) == NULL){
        printf("vg_init() error\n");
        return 1;
    }


	if (keyboard_subscribe() != 0) {
		printf("Error subscribing keyboard's interrupts\n");
		return 1;
	}


	if (vg_xpm(xpm, xi, yi) != 0) {

		printf("Error drawing xpm.\n");
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

				readOutBuf(&scancode);

				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (keyboard_unsubscribe() != 0) {
		printf("Error unsubscribing keyboard's interrupts\n");
		return 1;
	}

	if(vg_exit() != 0) {
		printf("Error exiting video graphics mode\n");
		return 1;
	}

	return 0;
}


int test_controller(){

	vbe_info_block_t vib_p;
    void * buf_base = lm_init();

    if (buf_base == NULL){
        printf("lm_init() error\n");
        return 1;
    }

	if (vbe_get_controller_info(&vib_p) != 0){
        printf("vbe_get_controller_info() error\n");
        return 1;
    }
	unsigned long vbe_version_bcd = vib_p.VbeVersion;
	unsigned long vbe_v_fst = 2 >> (vbe_version_bcd & 0xFF00);
	unsigned long vbe_v_sec = vbe_version_bcd & 0xFF;
	printf("%lu.%lu\n", vbe_v_fst, vbe_v_sec);


    uint16_t *farptr = (uint16_t *) (((vib_p.VideoModePtr & BASE_ADD) >> 12) + (vib_p.VideoModePtr & SEG_ADD) + buf_base) ;

	while (*farptr != VIDEO_PTR_STOP){
		printf("0x%X:", *farptr);
		farptr++;
	}
	printf("\n");

	unsigned long vbe_mem = 64 * vib_p.TotalMemory;

	printf("%lu\n", vbe_mem);

	return 0;

}

int test_move(char *xpm[], unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, short s, unsigned short f) {

	int r, ipc_status, irq_kbd = BIT(hook_id_kbd), irq_timer = BIT(hook_id_timer);
	message msg;
	unsigned long scancode = 0;
	int incrementx = 0, incrementy = 0, framesPassed = 0;
	Sprite * sprite;
	int dx, dy, counter = 0;
	dx = xf - xi;
	dy = yf - yi;

	if (f >= 60)
		f = 1;
	else
		f = 60/f;


	if (s > 0) {
		if (dx == 0) {
			if (dy < 0) {
				incrementy = -s;
			}else {
				incrementy = s;
			}
		}else if(dy == 0){
			if (dx < 0) {
				incrementx = -s;
			}else {
				incrementx = s;
			}
		}
	} else if (s < 0) {
		if (dx == 0) {
			if (dy < 0) {
				incrementy = -1;
			}else {
				incrementy = 1;
			}
		}else if(dy == 0){
			if (dx < 0) {
				incrementx = -1;
			}else {
				incrementx = 1;
			}
		}
	}

	if (timer_set_frequency(0, 60) != 0) {
		printf("Error setting timer's interrupts\n");
		return 1;
	}

	if (timer_subscribe() < 0){
		printf("Error subscribing timer's interrupts\n");
		return 1;
	}

	if (keyboard_subscribe() != 0){
		printf("Error subscribing keyboard's interrupts\n");
		return 1;
	}

	if (vg_init(MODE105) == NULL){
        printf("vg_init() error\n");
        return 1;
    }

	sprite = create_sprite(xpm, xi, yi, incrementx, incrementy);

    if (sprite == NULL){
        printf("Error creating sprite\n");
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
				if (msg.NOTIFY_ARG & irq_kbd) { /* subscribed interrupt */

				readOutBuf(&scancode);

				}
				if (msg.NOTIFY_ARG & irq_timer) {
                        counter++;
						if (counter == f) {
							if (framesPassed == s && s < 0) {
								sprite_move(sprite, xf, yf);
								framesPassed = 0;
								continue;
							}
							sprite_move(sprite, xf, yf);
							counter = 0;
							framesPassed++;
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

	if (timer_set_frequency(0, 60) != 0) {
		printf("Error setting timer's interrupts\n");
		return 1;
	}

	if (keyboard_unsubscribe() != 0){
		printf("Error unsubscribing keyboard's interrupts\n");
		return 1;
	}

	if (timer_unsubscribe() != 0){
		printf("Error unsubscribing timer's interrupts\n");
		return 1;
	}

	if(vg_exit() != 0) {

		printf("Error exiting video graphics mode\n");
		return 1;
	}
	return 0;
}
