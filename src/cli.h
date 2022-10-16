#ifndef YOBEMAG_CLI_H
#define YOBEMAG_CLI_H

#include <stdint.h>

#include "logging.h"
#include "errcodes.h"

typedef struct CLIArguments {
    LoggingLevel logging_level;
    const char* rom_path;
} CLIArguments;

/**
 * @brief   Allocates a struct to store CLI arguments in and returns a pointer to that struct
 *
 * @warning The allocated memory must be freed by using @p cli_config_destroy 
 * @returns Pointer to a CLIArguments struct
 */
CLIArguments* cli_config_default(void);

/**
 * @brief   Deallocates the memory, where @p conf points to
 *
 * @param   conf    Pointer to @p CLIArguments
 */
void cli_config_destroy(CLIArguments *const conf);

/**
 * @brief   Extract CLI flags with (optional) arguments from @p argv
 *
 * @param   conf    Pointer to @p CLIArguments
 * @param   argc    Number of arguments passed to the program
 * @param   argv    The passed arguments
 *
 * @returns An error code
 * @retval  ERR_SUCCESS on success
 * @retval  ERR_FAILURE otherwise
 */
ErrorCode cli_config_handle(CLIArguments *const conf, const int argc, char **const argv);

#endif //YOBEMAG_CLI_H
