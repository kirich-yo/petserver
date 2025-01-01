#ifndef _PET_LOGGER_H_
#define _PET_LOGGER_H_

#include <stdio.h>

/* TODO: Design and implement the logger */

enum PetLogLevel {
	PL_DEBUG,
	PL_INFO,
	PL_WARN,
	PL_ERROR,
	PL_FATAL
};

typedef struct _PetLogger {
	FILE * output;
	enum PetLogLevel log_level;
} PetLogger;

#endif /* _PET_LOGGER_H_ */
