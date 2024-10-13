#ifndef TIMEMATH_H
#define TIMEMATH_H

void usage();

typedef enum TimeFormat {
	DDMMYYYY,
	MMDDYYYY,
	YYYYMMDD,
	YYYYDDMM,
	MMYYYYDD,
	DDYYYYMM
} TimeFormat;

struct tm parsetime(const char *const str, int format);

#endif // TIMEMATH_H
