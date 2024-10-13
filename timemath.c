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

// In seconds
#define YEAR 3.15576e+7
#define MONTH 2.628e+6
#define WEEK 604800.0
#define DAY 86400.0
#define HOUR 3600.0
#define MINUTE 60.0

#define FORMAT_OPTION "-format"

int main(int argc, char** argv)
{
	argv++;
	argc--;

	if (argc < 2) {
		usage();
		return -1;
	}

	char* dateargs[2];
	int datearg = 0;

	int format = DDMMYYYY;

	for (int i = 0; i < argc; ++i) {

		if (strcmp(argv[i], FORMAT_OPTION) == 0) {
			i++;
			if (i < argc) {
				if (strcmp(argv[i], "DDMMYYYY") == 0) {
					format = DDMMYYYY;
				}
				else if (strcmp(argv[i], "MMDDYYYY") == 0) {
					format = MMDDYYYY;
				}
				else if (strcmp(argv[i], "YYYYMMDD") == 0) {
					format = YYYYMMDD;
				}
				else if (strcmp(argv[i], "YYYYDDMM") == 0) {
					format = YYYYDDMM;
				}
				else if(strcmp(argv[i], "DDYYYYMM") == 0) {
					format = DDYYYYMM;
				}
				else if(strcmp(argv[i], "MMDDDDYY") == 0) {
					format = MMYYYYDD;
				}
				else {
					fprintf(stderr, "error: invalid or unsupported format '%s'\n", argv[i]);
					exit(-1);
				}
			}
			else {
				fprintf(stderr, "error: expected date format after %s\n", FORMAT_OPTION);
				exit(-1);
			}
		}
		else {
			// Checking if argument isa date or not
			for(int j = 0; j < strlen(argv[i]); ++j) {
				if(!isdigit(argv[i][j]) && argv[i][j] != '/') {
					usage();
					exit(0);
				}
			}

			dateargs[datearg++] = argv[i];
		}
	}

	struct tm now = parsetime(dateargs[0], format);
	struct tm then = parsetime(dateargs[1], format);

	now.tm_isdst = -1;
	then.tm_isdst = -1;

	int year1 = now.tm_year;
	int year2 = then.tm_year;

	// We're only doing the day and month calculations.
	now.tm_year = 70;
	then.tm_year = 70;

	double years = fabs(year2 - year1);

	time_t nowtime = mktime(&now);
	time_t thentime = mktime(&then);

	double diff = fabs((double)(thentime - nowtime));

	double seconds = diff + (years * YEAR);
	double minutes = (seconds / MINUTE);
	double hours = (seconds / HOUR);
	double days = (seconds / DAY);
	double weeks = (seconds / WEEK);
	double months = (seconds / MONTH);
	double actualyears = (seconds / YEAR);

	putc('\n', stdout);
	printf("seconds: %.0f\n", seconds);
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

	int yearMember, dayMember, monthMember;

	switch (format) {
	case DDMMYYYY: {
		dayMember = 0;
		monthMember = 1;
		yearMember = 2;
	} break;
	case MMDDYYYY: {
		dayMember = 1;
		monthMember = 0;
		yearMember = 2;
	} break;
	case YYYYMMDD: {
		yearMember = 0;
		monthMember = 1;
		dayMember = 2;
	} break;
	case YYYYDDMM: {
		yearMember = 0;
		monthMember = 2;
		dayMember = 1;
	} break;
	case MMYYYYDD: {
		yearMember = 1;
		monthMember = 0;
		dayMember = 2;
	} break;
	case DDYYYYMM: {
		yearMember = 1;
		monthMember = 2;
		dayMember = 0;
	} break;
	default: {
		assert(false && "unreachable");
		exit(-1);
	} break;
	}

	members[dayMember] = &res.tm_mday;
	members[monthMember] = &res.tm_mon;
	members[yearMember] = &res.tm_year;

	int member = 0;

	unsigned char numbuf[4] = { 0 };

	int digits = 0;
	int reset = 0;

	for (int i = 0; i < strlen(str); ++i) {

		if (str[i] == '/' || reset == 1) {

			if (digits == 0) {
				fprintf(stderr, "error: incomplete date\n");
				exit(-1);
			}
			
			int num = 0;
			for (int j = 0; j < digits; ++j) {
				num += numbuf[j] * (int)pow(10, (digits-j-1));
			}

			if (digits > 2 && member != yearMember) {
				fprintf(stderr, "error: expected non-year component with size lower than 3, got '%d' with size '%d'\n", num, digits);
				exit(-1);
			}
			else if (digits > 4 && member == yearMember) {
				fprintf(stderr, "error: expected year component with size lower than 5, got '%d' with size '%d'\n", num, digits);
				exit(-1);
			}

			*members[member++] += num;
			digits = 0;
		}
		else if (isdigit(str[i])) {
			numbuf[digits++] = (str[i] - '0');

			// For when a date doesn't end with '/'
			if (i == strlen(str) - 1) {
				i -= 1;
				reset = 1;
				continue;
			}
		}
		else {
			fprintf(stderr, "error: unexpected character '%c'\n", str[i]);
			exit(-1);
		}
	}

	if (member != 3) {
		fprintf(stderr, "error: incomplete date\n");
		exit(-1);
	}

	if (res.tm_year < -1900) {
		fprintf(stderr, "error: invalid year '%d'\n", res.tm_year);
		exit(-1);
	}

	if (res.tm_mon < 0 || res.tm_mon > 11) {
		fprintf(stderr, "error: invalid month '%d', expected value between 0-12\n", res.tm_mon+1);
		exit(-1);
	}

	if (res.tm_mday < 0 || res.tm_mday > monthLengths[res.tm_mon]) {
		fprintf(stderr, "error: invalid day '%d', expected value between 0-%d\n", res.tm_mday+1, monthLengths[res.tm_mon]);
		exit(-1);
	}

	return res;
}

void usage()
{
	putc('\n', stdout);
	printf("Usage: timemath <date> <date> [options]\n");
	printf("options:\n");
	printf("\t-format ");
	printf("\t\tDDMMYYYY (default), MMDDYYYY, YYYYMMDD, YYYYDDMM, MMYYYYDD, DDYYYYMM\n");
}
