#ifndef YOBEMAG_LOGGING_H
#define YOBEMAG_LOGGING_H

#include <stdio.h>

#include "cli.h"

typedef enum LoggingLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
} LoggingLevel;

void log_str(const LoggingLevel dbg_lvl, const char* const dbg_lvl_str, FILE *const stream, const char* const msg, ...);

void log_set_lvl(CLIArguments *const conf);

#define LOG_DEBUG(msg, ...)     log_str(DEBUG, "DEBUG", stdout, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...)      log_str(INFO, "INFO", stdout, msg, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...)   log_str(WARNING, "WARNING", stdout, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...)     log_str(ERROR, "ERROR", stderr, msg, ##__VA_ARGS__)
#define LOG_FATAL(msg, ...)     log_str(FATAL, "FATAL", stderr, msg, ##__VA_ARGS__)

#endif //YOBEMAG_LOGGING_H
