#ifndef _PET_LOGGER_H_
#define _PET_LOGGER_H_

#include <stdio.h>

/* TODO: Design and implement the logger */

static const char * PET_LOG_LEVEL_STRINGS[] = {
	"DEBUG:",
	"INFO:",
	"WARN:",
	"ERROR:",
	"FATAL:"
};

enum PetLogLevel {
	PL_DEBUG,
	PL_INFO,
	PL_WARN,
	PL_ERROR,
	PL_FATAL,
};

typedef const char * (*PetLogFormatter)(enum PetLogLevel log_level);

typedef struct _PetLogger {
	FILE * std_output, * err_output;
	enum PetLogLevel log_level;
	PetLogFormatter log_formatter;
} PetLogger;

PetLogger * Pet_NewFileLogger(const char * std_filename, const char * err_filename, enum PetLogLevel log_level);

PetLogger * Pet_NewStdoutLogger(enum PetLogLevel log_level);

const char * Pet_DefaultLogFormatter(enum PetLogLevel log_level);

const char * Pet_PrettyLogFormatter(enum PetLogLevel log_level);

const char * Pet_JSONLogFormatter(enum PetLogLevel log_level);

void Pet_Log(PetLogger * log, enum PetLogLevel log_level, const char * text);

void Pet_FmtLog(PetLogger * log, enum PetLogLevel log_level, const char * fmt, ...);

void Pet_FreeLogger(PetLogger * log);

#endif /* _PET_LOGGER_H_ */
