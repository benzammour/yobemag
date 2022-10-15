#ifndef _CRETRO_SRC_CLI_H_
#define _CRETRO_SRC_CLI_H_

#include <stdint.h>

typedef struct arg_conf {
    int debug;
    int us_delay;
    const char* rom_path;
} CLIArguments;

CLIArguments* cli_config_default(void);

void cli_config_destroy(CLIArguments *conf);

int cli_config_handle(CLIArguments* const conf, int argc, char **argv);

#endif /* _CRETRO_SRC_CLI_H_ */
