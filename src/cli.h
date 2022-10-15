#ifndef YOBEMAG_CLI_H
#define YOBEMAG_CLI_H

#include <stdint.h>

typedef struct arg_conf {
    int debug;
    int us_delay;
    const char* rom_path;
} CLIArguments;

CLIArguments* cli_config_default(void);

void cli_config_destroy(CLIArguments *conf);

int cli_config_handle(CLIArguments* const conf, int argc, char **argv);

#endif //YOBEMAG_CLI_H
