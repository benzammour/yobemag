#ifndef YOBEMAG_CLI_H
#define YOBEMAG_CLI_H

#include <stdint.h>

#include "logging.h"

/**
 * @brief Stores passed cli arguments for later use.
 */
typedef struct CLIArguments {
	/**
	 * @brief Indicates the minimum logging level (checked by ::log_str()).
	 */
    LoggingLevel logging_level;
	/**
	 * @brief Propagated to ::rom_load() to create a memory map to rom file.
	 */
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
