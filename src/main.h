#ifndef MAIN_H
#define MAIN_H

void skiparg(int *argc, char **argv);

typedef enum TimeFormat {
	DDMMYY,
	MMDDYY
} TimeFormat;

struct tm parsetime(const char *const str, int format);

#endif // MAIN_H