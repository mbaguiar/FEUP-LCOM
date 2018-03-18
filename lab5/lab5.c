#include "test5.h"
#include "pixmap.h"
#include <limits.h>
#include <strings.h>
#include <errno.h>
#include <minix/drivers.h>

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char **argv);

int main(int argc, char **argv)
{
	/* DO NOT FORGET TO initialize service */
	sef_startup();

	sys_enable_iop(SELF);

	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"init <hexadecimal no.- mode> <decimal no.- delay> \"\n"
			"\t service run %s -args \"square <decimal no.- x> <decimal no.- y> <decimal no.- size> <decimal no.- color>\"\n"
			"\t service run %s -args \"line <decimal no.- xi> <decimal no.- yi> <decimal no.- xf> <decimal no.- yf> <decimal no.- color>\"\n"
			"\t service run %s -args \"xpm <string- xpm> <decimal no.- xi> <decimal no.- yi>\"\n",
			"\t service run %s -args \"move <string xpm> <decimal no.- xi> <decimal no.- yi> <decimal no.- xf> <decimal no.- yf> <decimal no.- speed> <decimal no.- frame rate>\"\n"
			"\t service run %s -args \"controller\"\n",
			argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	char ** xpm;
	short speed;
	unsigned short mode, delay, x, y, size, color, xi, yi, xf, yf, color2, frame_rate;
	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if (argc != 4) {
			printf("video: wrong no. of arguments for video_test_init()\n");
			return 1;
		}
		mode = parse_ulong(argv[2], 16);						/* Parses string to unsigned long */
        delay = parse_ulong(argv[3], 10);
		if ((mode == ULONG_MAX) || (delay == ULONG_MAX))
			return 1;
		printf("video::video_test_init(%lu, %lu)\n", mode, delay);
		return video_test_init(mode, delay);
		return 0;
	}
	else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
			printf("video: wrong no. of arguments for video_test_square()\n");
			return 1;
		}
    x = parse_ulong(argv[2], 10);
    y = parse_ulong(argv[3], 10);
    size = parse_ulong(argv[4], 10);
    color = parse_ulong(argv[5], 16);
    if ((x == ULONG_MAX) || (y == ULONG_MAX) || (size == ULONG_MAX) || (color == ULONG_MAX))
			return 1;
		printf("video::video_test_square(%lu, %lu, %lu, %lu)\n", x, y, size, color);
		return video_test_square(x, y, size, color);
	}
	else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf("video: wrong no. of arguments for video_test_line\n");
			return 1;
		}
		xi = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
   		yi = parse_ulong(argv[3], 10);
		xf = parse_ulong(argv[4], 10);
    	yf = parse_ulong(argv[5], 10);
    color2 = parse_ulong(argv[6], 16);
		if ((xi == ULONG_MAX) || (yi == ULONG_MAX) || (xf == ULONG_MAX) || (yf == ULONG_MAX) || (color2 == ULONG_MAX))
			return 1;
		printf("video::video_test_line(%lu, %lu, %lu, %lu, %lu)\n", xi, yi, xf, yf, color2);
		return video_test_line(xi, yi, xf, yf, color2);
	}
	else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0){

		if (argc != 5){
			printf("video: wrong no. of arguments for video_test_xpm\n");
			return 1;
		}

		xi = parse_ulong(argv[3], 10);
		yi = parse_ulong(argv[4], 10);

		if ((xi == ULONG_MAX) || (yi == ULONG_MAX)) return 1;

		if (strncmp(argv[2], "pic1", strlen("pic1")) == 0) xpm = pic1;
		else if(strncmp(argv[2], "pic2", strlen("pic2")) == 0) xpm = pic2;
		else if(strncmp(argv[2], "cross", strlen("cross")) == 0) xpm = cross;
		else if(strncmp(argv[2], "pic3", strlen("pic3")) == 0) xpm = pic3;
		else if(strncmp(argv[2], "penguin", strlen("penguin")) == 0) xpm = penguin;
		else {
			printf("video: use one of the following xpm: pic1, pic2, pic3, cross, penguin\n");
			return 1;
		}
		printf("video::test_xpm(%s, %lu, %lu)\n", argv[2], xi, yi);
		return test_xpm(xpm, xi, yi);
	}
	else if (strncmp(argv[1], "move", strlen("move")) == 0){

		if (argc != 9){
			printf("video: wrong no. of arguments for video_test_move\n");
			return 1;
		}

		xi = parse_ulong(argv[3], 10);
		yi = parse_ulong(argv[4], 10);
		xf = parse_ulong(argv[5], 10);
		yf = parse_ulong(argv[6], 10);
		speed = parse_long(argv[7], 10);
		frame_rate = parse_ulong(argv[8], 10);

		if ((xi == ULONG_MAX) || (yi == ULONG_MAX) || (xf == ULONG_MAX) || (yf == ULONG_MAX) || (speed == LONG_MAX) || (frame_rate == ULONG_MAX)) return 1;

		if (strncmp(argv[2], "pic1", strlen("pic1")) == 0) xpm = pic1;
		else if(strncmp(argv[2], "pic2", strlen("pic2")) == 0) xpm = pic2;
		else if(strncmp(argv[2], "cross", strlen("cross")) == 0) xpm = cross;
		else if(strncmp(argv[2], "pic3", strlen("pic3")) == 0) xpm = pic3;
		else if(strncmp(argv[2], "penguin", strlen("penguin")) == 0) xpm = penguin;
		else {
			printf("video: use one of the following xpm: pic1, pic2, pic3, cross, penguin\n");
			return 1;
		}
		printf("video::test_move(%s, %lu, %lu, %lu, %lu, %ld, %lu)\n", argv[2], xi, yi, xf, yf, speed, frame_rate);
		return test_move(xpm, xi, yi, xf, yf, speed, frame_rate);
	}
	else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
		if (argc != 2) {
			printf("video: wrong no. of arguments for test_controller()\n");
			return 1;
		}
		printf("video::test_controller()\n");
		return test_controller();
	}
	else {
		printf("video: %s - no valid function!\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base)
{
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("video: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

static long parse_long(char *str, int base) {
    char *endptr;
    unsigned long val;

    val = strtol(str, &endptr, base);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
        || (errno != 0 && val == 0)) {
        perror("strtol");
        return LONG_MAX;
    }

    if (endptr == str) {
        printf("mouse: parse_long: no digits were found in %s \n", str);
        return LONG_MAX;
    }

     /* Successful conversion */
  return val;
}
