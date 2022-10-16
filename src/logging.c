#include <stdarg.h>
#include <time.h>

#include "logging.h"

/******************************************************
 *** LOCAL VARIABLES                                ***
 ******************************************************/

static LoggingLevel min_log_lvl = FATAL;

/******************************************************
 *** EXPOSED METHODS                                ***
 ******************************************************/

void log_set_lvl(LoggingLevel log_lvl) {
    min_log_lvl = log_lvl;

    // minimum debug level cannot be higher than fatal
    if (min_log_lvl > FATAL)
        min_log_lvl = FATAL;

    // minimum debug level cannot be lower than debug
    if (min_log_lvl < DEBUG)
        min_log_lvl = DEBUG;

    LOG_INFO("Log level initialized to %d.", min_log_lvl);
}

__attribute__((format(printf, 4, 5)))
void log_str(LoggingLevel const log_lvl, const char *const log_lvl_str, FILE *const stream, const char *const msg, ...) {
    if (log_lvl < min_log_lvl) return;

    va_list args;
    time_t current_time;
    struct tm local_time;
    char time_str[20];

    va_start(args, msg);

    time(&current_time);
    localtime_r(&current_time, &local_time);
    strftime(time_str, 20, "%F %T", &local_time);

    fprintf(stream, "%10s – %s – ", log_lvl_str, time_str);
    vfprintf(stream, msg, args);
    fprintf(stream, "\n");
}

