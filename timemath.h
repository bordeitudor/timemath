#ifndef TIMEMATH_H
#define TIMEMATH_H

void skiparg(int *argc, char **argv);

// TODO: Implement support for formats other than DDMMYY and add commandline arguments for them

typedef enum TimeFormat {
	DDMMYY,
	MMDDYY
} TimeFormat;

struct tm parsetime(const char *const str, int format);

#endif // TIMEMATH_H