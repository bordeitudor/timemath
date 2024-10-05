#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "timemath.h"

static int monthLengths[] = {
	31,
	28,
	31,
	30,
	31,
	30,
	31,
	31,
	30,
	31,
	30,
	31
};

int main(int argc, char** argv)
{
	argv++;
	argc--;

	if (argc != 2) exit(-1);

	struct tm now = parsetime(argv[0], DDMMYY);
	struct tm then = parsetime(argv[1], DDMMYY);

	// TODO: Handle wrong dates

	now.tm_isdst = -1;
	then.tm_isdst = -1;

	int year1 = now.tm_year;
	int year2 = then.tm_year;
	
	now.tm_year = 70;
	then.tm_year = 70;

	double years = fabs(year2 - year1);

	double extra = 86400.0 * (fabs(monthLengths[now.tm_mon-1] - monthLengths[then.tm_mon-1]));

	time_t nowtime = mktime(&now);
	time_t thentime = mktime(&then);

	double diff = fabs((double)thentime - (double)nowtime);

	double seconds = (years * 3.154e+7) + diff + extra;
	double minutes = (seconds / 60.0);
	double hours = (seconds / 3600.0);
	double days = (seconds / 86400.0);
	double weeks = (seconds / 604800.0);
	double months = (seconds / 2.628e+6);
	double actualyears = (seconds / 3.154e+7);

	putc('\n', stdout);
	printf("second:  %.1f\n", seconds);
	printf("minutes: %.1f\n", minutes);
	printf("hours:   %.1f\n", hours);
	printf("days:    %.1f\n", days);
	printf("weeks:   %.1f\n", weeks);
	printf("months:  %.1f\n", months);
	printf("years:   %.1f\n", actualyears);

	return 0;
}

struct tm parsetime(const char* const str, int format)
{
	struct tm res = { 0 };

	int* members[3];

	res.tm_mon = -1;
	res.tm_year = -1900;
	res.tm_mday = 0;

	switch (format) {
	case DDMMYY: {
		members[0] = &res.tm_mday;
		members[1] = &res.tm_mon;
		members[2] = &res.tm_year;
	} break;
	case MMDDYY: {
		members[0] = &res.tm_mon;
		members[1] = &res.tm_mday;
		members[2] = &res.tm_year;
	} break;

	default: {
		exit(-2);
	} break;
	}

	int member = 0;

	unsigned char numbuf[4] = { 0 };

	int digits = 0;

	for (int i = 0; i < ((str[strlen(str)-1]=='/')?strlen(str):strlen(str)+1); ++i) {
		if (str[i] == '/' || i+1 > strlen(str)) {
			switch (digits) {
			case 1: {
				if (member == 2) exit(-1);
				*members[member++] = numbuf[0];
			} break;

			case 2: {
				if (member == 2) exit(-1);
				*members[member++] = numbuf[0] * 10 + numbuf[1];
			} break;

			case 4: {
				int year = (numbuf[0] * 1000 + numbuf[1] * 100 + numbuf[2] * 10 + numbuf[3]);
				*members[member++] = year;
			} break;
			default: {
				exit(-1);
			} break;
			}
			digits = 0;
		}
		else
			if (isdigit(str[i])) {
				numbuf[digits++] = (str[i] - '0');
			}
			else {
				fprintf(stderr, "error: couldn't parse time, unexpected character '%c'\n", str[i]);
				exit(-1);
			}
	}

	return res;
}