#ifndef _CH8_SRC_DEBUG_H_
#define _CH8_SRC_DEBUG_H_

#include <stdio.h>

#include "cli.h"

typedef enum log_level {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
} LoggingLevel;

void log_str(LoggingLevel dbg_lvl, const char* dbg_lvl_str, FILE* stream, const char* msg, ...);

void log_set_lvl(const CLIArguments* conf);

#define LOG_DEBUG(msg, ...)     log_str(DEBUG, "DEBUG", stdout, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...)      log_str(INFO, "INFO", stdout, msg, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...)   log_str(WARNING, "WARNING", stdout, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...)     log_str(ERROR, "ERROR", stderr, msg, ##__VA_ARGS__)
#define LOG_FATAL(msg, ...)     log_str(FATAL, "FATAL", stderr, msg, ##__VA_ARGS__)

#endif /* _CH8_SRC_DEBUG_H_ */
