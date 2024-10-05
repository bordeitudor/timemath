#ifndef TIMEMATH_H
#define TIMEMATH_H

void usage();

// TODO: Implement support for formats other than DDMMYY and add commandline arguments for them

typedef enum TimeFormat {
	DDMMYYYY,
	MMDDYYYY,
	YYYYMMDD,
	YYYYDDMM
} TimeFormat;

struct tm parsetime(const char *const str, int format);

#endif // TIMEMATH_H