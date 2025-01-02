#include "logger.h"

#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

PetLogger * Pet_NewFileLogger(const char * std_filename, const char * err_filename, enum PetLogLevel log_level) {
	PetLogger * new_logger = (PetLogger *)malloc(sizeof(PetLogger));
	FILE * log_file = fopen(std_filename, "w");
	FILE * log_err_file = fopen(err_filename, "w");

	new_logger->std_output = log_file;
	new_logger->err_output = log_err_file;
	new_logger->log_level = log_level;
	new_logger->log_formatter = Pet_DefaultLogFormatter;

	return new_logger;
}

PetLogger * Pet_NewStdoutLogger(enum PetLogLevel log_level) {
	PetLogger * new_logger = (PetLogger *)malloc(sizeof(PetLogger));

	new_logger->std_output = stdout;
	new_logger->err_output = stderr;
	new_logger->log_level = log_level;
	new_logger->log_formatter = Pet_DefaultLogFormatter;

	return new_logger;
}

const char * Pet_DefaultLogFormatter(enum PetLogLevel log_level) {
	char * fmt_buff;
	char fmt_time[20];

	time_t unix_time;
	time(&unix_time);

	struct tm * tm_time = (struct tm *)malloc(sizeof(struct tm));
	localtime_r(&unix_time, tm_time);
	strftime(fmt_time, 20, "%d.%m.%Y %H:%M:%S", tm_time);
	// free(tm_time);

	size_t fmt_buff_size = snprintf(NULL, 0, "[%s] %6s %%s", fmt_time, PET_LOG_LEVEL_STRINGS[log_level]);
	fmt_buff = (char *)malloc(fmt_buff_size);
	sprintf(fmt_buff, "[%s] %6s %%s\n", fmt_time, PET_LOG_LEVEL_STRINGS[log_level]);

	return fmt_buff;
}

const char * Pet_PrettyLogFormatter(enum PetLogLevel log_level) {
	char * fmt_buff;
	char fmt_time[20];

	time_t unix_time;
	time(&unix_time);

	struct tm * tm_time = (struct tm *)malloc(sizeof(struct tm));
	localtime_r(&unix_time, tm_time);
	strftime(fmt_time, 20, "%d.%m.%Y %H:%M:%S", tm_time);
	// free(tm_time);

	size_t fmt_buff_size = snprintf(NULL, 0, "%s[%s] %s%s%6s%s %s%%s%s\n",
			PET_LOG_DARK_GRAY,
			fmt_time,
			PET_LOG_WHITE_BOLD,
			PET_LOG_LEVEL_COLORS[log_level],
			PET_LOG_LEVEL_STRINGS[log_level],
			PET_LOG_RESET_COLOR,
			PET_LOG_TEXT_COLORS[log_level],
			PET_LOG_RESET_COLOR
		);
	fmt_buff = (char *)malloc(fmt_buff_size);
	sprintf(fmt_buff, "%s[%s] %s%s%6s%s %s%%s%s\n",
			PET_LOG_DARK_GRAY,
			fmt_time,
			PET_LOG_WHITE_BOLD,
			PET_LOG_LEVEL_COLORS[log_level],
			PET_LOG_LEVEL_STRINGS[log_level],
			PET_LOG_RESET_COLOR,
			PET_LOG_TEXT_COLORS[log_level],
			PET_LOG_RESET_COLOR
		);

	return fmt_buff;
}

const char * Pet_JSONLogFormatter(enum PetLogLevel log_level) {
	char * fmt_buff;
	char fmt_time[21];

	time_t unix_time;
	time(&unix_time);

	struct tm * tm_time = (struct tm *)malloc(sizeof(struct tm));
	localtime_r(&unix_time, tm_time);
	strftime(fmt_time, 21, "%Y-%m-%dT%H:%M:%SZ", tm_time);
	// free(tm_time);

	size_t fmt_buff_size = snprintf(NULL, 0, "{\"time\": \"%s\", \"log_level\": \"%-5s\", \"text\": \"%%s\"}\n", fmt_time, PET_LOG_LEVEL_STRINGS[log_level]);
	fmt_buff = (char *)malloc(fmt_buff_size);
	sprintf(fmt_buff, "{\"time\": \"%s\", \"log_level\": \"%-5s\", \"text\": \"%%s\"}\n", fmt_time, PET_LOG_LEVEL_STRINGS[log_level]);

	return fmt_buff;
}

void Pet_Log(PetLogger * log, enum PetLogLevel log_level, const char * str) {
	if (log->log_level > log_level) return;

	const char * fmt = log->log_formatter(log_level);
	fprintf(log_level < (int)PL_ERROR ? log->std_output : log->err_output, fmt, str);
	free(fmt);
}

void Pet_FmtLog(PetLogger * log, enum PetLogLevel log_level, const char * fmt, ...) {
	if (log->log_level > log_level) return;

	va_list fmt_args;
	va_start(fmt_args, fmt);

	char * final_fmt;
	size_t final_fmt_size = vsnprintf(NULL, 0, fmt, fmt_args);
	final_fmt = (char *)malloc(final_fmt_size);

	va_start(fmt_args, fmt);
	vsprintf(final_fmt, fmt, fmt_args);

	const char * base_fmt = log->log_formatter(log_level);
	fprintf(log_level < (int)PL_ERROR ? log->std_output : log->err_output, base_fmt, final_fmt);
	free(base_fmt);
	free(final_fmt);
}

void Pet_FreeLogger(PetLogger * log) {
	if (log->std_output != stdout)
		fclose(log->std_output);
	if (log->err_output != stderr)
		fclose(log->err_output);
	free(log);
}
