#ifndef YOBEMAG_CLI_H
#define YOBEMAG_CLI_H

#include <stdint.h>

#include "logging.h"
#include "err_codes.h"

typedef struct CLIArguments {
    LoggingLevel logging_level;
    const char* rom_path;
} CLIArguments;

/**
 * @brief   Extract CLI flags with (optional) arguments from @p argv
 *
 * @param   argc    Number of arguments passed to the program
 * @param   argv    The passed arguments
 */
CLIArguments* cli_config_handle(int argc, char **argv);

#endif //YOBEMAG_CLI_H
