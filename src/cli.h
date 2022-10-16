#ifndef YOBEMAG_CLI_H
#define YOBEMAG_CLI_H

#include <stdint.h>

#include "logging.h"
#include "errcodes.h"

typedef struct CLIArguments {
    LoggingLevel logging_level;
    const char* rom_path;
} CLIArguments;

CLIArguments* cli_config_default(void);

void cli_config_destroy(CLIArguments *const conf);

/**
 * @brief   Extract CLI flags with (optional) arguments from @p argv
 *
 * @param   conf    Pointer to @CLIArguments
 * @param   argc    Number of arguments passed to the program
 * @param   argv    The passed arguments
 *
 * @returns An error code
 * @retval  ERR_SUCCESS on success
 * @retval  ERR_FAILURE otherwise
 */
ErrorCode cli_config_handle(CLIArguments *const conf, const int argc, char **const argv);

#endif //YOBEMAG_CLI_H
