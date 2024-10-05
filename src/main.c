#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"

int main(int argc, char** argv)
{
	argv++;
	argc--;

	if (argc != 2) exit(-1);

	struct tm now = parsetime(argv[0], DDMMYY);
	struct tm then = parsetime(argv[1], DDMMYY);

	int year1 = now.tm_year;
	int year2 = then.tm_year;
	
	now.tm_year = 70;
	then.tm_year = 70;

	double time = difftime(mktime(&then), mktime(&now));

	double seconds = fabs(time);
	double minutes = fabs(seconds / 60);
	double hours = fabs(minutes / 60);
	double days = fabs(hours / 24);
	double months = fabs(days / 30);
	double years = fabs(year2 - year1);

	printf("%.f seconds\n", seconds);
	printf("%.f minutes\n", minutes);
	printf("%.f hours\n", hours);
	printf("%.f days\n", days);
	printf("%.f months\n", months);
	printf("%.f years\n", years);

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
				*members[member++] += numbuf[0];
			} break;

			case 2: {
				if (member == 2) exit(-1);
				*members[member++] += numbuf[0] * 10 + numbuf[1];
			} break;

			case 4: {
				int year = (numbuf[0] * 1000 + numbuf[1] * 100 + numbuf[2] * 10 + numbuf[3]);
				*members[member++] += year;
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