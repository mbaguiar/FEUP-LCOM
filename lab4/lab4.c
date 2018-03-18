//#include "timer.h"
//#include "i8254.h"
#include "test4.h"
#include <limits.h>
#include <string.h>
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

	if (argc == 1) {	 				/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"packet <decimal no.- counter>\"\n"
			"\t service run %s -args \"async <decimal no. - time>\"\n"
			"\t service run %s -args \"remote <decimal no. - period> <decimal no. - counter>\"\n"
			"\t service run %s -args \"gesture <decimal no. - length>\"\n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned long counter1, time, period, counter2, length;
	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for mouse_test_packet()\n");
			return 1;
		}
		counter1 = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (counter1 == ULONG_MAX)
			return 1;
		printf("mouse::mouse_test_packet(%lu)\n", counter1);
		return mouse_test_packet(counter1);
	}
	else if (strncmp(argv[1], "async", strlen("async")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for mouse_test_async()\n");
			return 1;
		}
		time = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (time == ULONG_MAX)
			return 1;
		printf("mouse::mouse_test_async(%lu)\n", time);
		return mouse_test_async(time);
	}
	else if (strncmp(argv[1], "remote", strlen("remote")) == 0) {
		if (argc != 4) {
			printf("mouse: wrong no of arguments for mouse_test_remote()\n");
			return 1;
		}
		period = parse_ulong(argv[2], 10);
		counter2 = parse_ulong(argv[3], 10);					/* Parses string to unsigned long */
		if ((period == ULONG_MAX) && (counter2 == ULONG_MAX))
			return 1;
		printf("mouse::mouse_test_remote(%lu, %lu)\n", period, counter2);
		return mouse_test_remote(period, counter2);
	}
	else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for mouse_test_gesture()\n");
			return 1;
		}
		length = parse_long(argv[2], 10);						/* Parses string to unsigned long */
		if (length == LONG_MAX)
			return 1;
        printf("mouse::mouse_test_gesture(%ld)\n", length);
		return mouse_test_gesture(length);
	}
	else {
		printf("mouse: %s - no valid function!\n", argv[1]);
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
		printf("mouse: parse_ulong: no digits were found in %s\n", str);
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