#include <stdarg.h>
#include <time.h>

#include "logging.h"


/******************************************************
 *** LOCAL DEFINES                                  ***
 ******************************************************/

/******************************************************
 *** LOCAL VARIABLES                                ***
 ******************************************************/

static LoggingLevel min_dbg_lvl = FATAL;

/******************************************************
 *** LOCAL METHODS                                  ***
 ******************************************************/

/******************************************************
 *** EXPOSED METHODS                                ***
 ******************************************************/

void log_set_lvl(CLIArguments *const conf) {
    min_dbg_lvl = (LoggingLevel) conf->logging_level;

    // minimum debug level cannot be higher than fatal
    if (min_dbg_lvl > FATAL)
        min_dbg_lvl = FATAL;

    // minimum debug level cannot be lower than debug
    if (min_dbg_lvl < DEBUG)
        min_dbg_lvl = DEBUG;

    LOG_INFO("Log level initialized to %d.", min_dbg_lvl);
}

__attribute__((format(printf, 4, 5)))
void log_str(LoggingLevel const dbg_lvl, const char *const dbg_lvl_str, FILE *const stream, const char *const msg, ...) {
    if (dbg_lvl < min_dbg_lvl) return;

    va_list args;
    time_t rawtime;
    struct tm * timeinfo;

    va_start(args, msg);
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char* time_str = asctime(timeinfo);
    time_str[24] = '\0'; // terminate str early to eliminate newline (i hate this)

    fprintf(stream, "%10s – %s – ", dbg_lvl_str, time_str);
    vfprintf(stream, msg, args);
    fprintf(stream, "\n");
}
