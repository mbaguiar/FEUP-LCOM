#include "test3.h"
#include <limits.h>
#include <strings.h>
#include <errno.h>

static int proc_args(int argc, char **argv);
static unsigned long parse_ulong(char *str, int base);
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
			"\t service run %s -args \"ih <decimal no.- mode>\"\n"
			"\t service run %s -args \"polling\"\n"
			"\t service run %s -args \"time <decimal no. - time>\"\n",
			argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned short mode, time;
	if (strncmp(argv[1], "ih", strlen("ih")) == 0) {
		if (argc != 3) {
			printf("keyboard: wrong no. of arguments for kbd_test_scan\n");
			return 1;
		}
		mode = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (mode == ULONG_MAX)
			return 1;
		printf("keyboard::kbd_test_scan(%lu)\n", mode);
		return kbd_test_scan(mode);
	}
	else if (strncmp(argv[1], "poll", strlen("poll")) == 0) {
		if (argc != 2) {
			printf("timer: wrong no. of arguments for timer_test_time_base()\n");
			return 1;
		}
		printf("keyboard::kbd_test_poll()\n");
		return kbd_test_poll();
	}
	else if (strncmp(argv[1], "time", strlen("time")) == 0) {
		if (argc != 3) {
			printf("keyboard: wrong no. of arguments for kbd_test_timed_sacn\n");
			return 1;
		}
		time = parse_ulong(argv[2], 10);						/* Parses string to unsigned long */
		if (time == ULONG_MAX)
			return 1;
		printf("keyboard::kbd_test_timed_scan(%lu)\n", time);
		return kbd_test_timed_scan(time);
	}
	else {
		printf("keyboard: %s - no valid function!\n", argv[1]);
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
		printf("keyboard: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}
