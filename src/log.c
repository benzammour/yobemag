#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>

#include "log.h"

/******************************************************
 *** LOCAL VARIABLES                                ***
 ******************************************************/

#define MAX_TIME_STR_LEN (20)

static LoggingLevel min_log_lvl = FATAL;

/******************************************************
 *** LOCAL METHODS                                  ***
 ******************************************************/

static void get_time(char* const time_str) {
	time_t current_time;
	struct tm local_time;
	time(&current_time);
	localtime_r(&current_time, &local_time);
	strftime(time_str, MAX_TIME_STR_LEN, "%F %T", &local_time);
}

/******************************************************
 *** EXPOSED METHODS                                ***
 ******************************************************/

void log_set_lvl(LoggingLevel log_lvl) {
    min_log_lvl = log_lvl;

    // minimum logging level cannot be higher than fatal
	if (min_log_lvl > FATAL) {
		LOG_WARNING("Provided logging level %d is higher than FATAL (%d).", min_log_lvl, FATAL);
		min_log_lvl = FATAL;
	}

    // minimum logging level cannot be lower than debug
	if (min_log_lvl < DEBUG) {
		LOG_WARNING("Provided logging level %d is lower than DEBUG (%d).", min_log_lvl, DEBUG);
		min_log_lvl = DEBUG;
	}

    LOG_INFO("Log level initialized to %d.", min_log_lvl);
}

void log_exit(const char *const file_path, const int line_number, const char *const msg, ...) {
	va_list args;
	va_start(args, msg);

	char time_str[MAX_TIME_STR_LEN];
	get_time(time_str);

	char *file_path_copy = strdup(file_path);
	const char *file_name = basename(file_path_copy);

	fprintf(stderr, "%10s – %s – %s:%d - ", "FATAL", time_str, file_name, line_number);
	vfprintf(stderr, msg, args);
	fprintf(stderr, "\n");

	va_end(args);

    exit(EXIT_FAILURE);
}

void log_str(const LoggingLevel log_lvl, const char *const log_lvl_str, FILE *const stream, const char *const msg, ...) {
    if (log_lvl < min_log_lvl)
		return;

    va_list args;
	va_start(args, msg);

	char time_str[MAX_TIME_STR_LEN];
	get_time(time_str);

	fprintf(stream, "%10s – %s – ", log_lvl_str, time_str);
	vfprintf(stream, msg, args);
	fprintf(stream, "\n");

	va_end(args);
}
