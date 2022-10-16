#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <getopt.h>
#include <stdio.h>
#include <errno.h>

#include "cli.h"
#include "logging.h"

/******************************************************
 *** LOCAL VARIABLES                                ***
 ******************************************************/

static const char* usage_str = "yobemag [-l <0..4>] <ROM>";

/******************************************************
 *** LOCAL METHODS                                  ***
 ******************************************************/

static int safe_strtol(const char* const str_to_conv, int* const store_into) {
    char* end;
    const long strtol_in = strtol(str_to_conv, &end, 10);

    errno = 0;

    if (end == str_to_conv) {
        LOG_FATAL("%s: not a decimal number", str_to_conv);
    } else if ('\0' != *end) {
        LOG_FATAL("%s: extra characters at end of input: %s", str_to_conv, end);
    } else if ((LONG_MIN == strtol_in || LONG_MAX == strtol_in) && ERANGE == errno) {
        LOG_FATAL("%s out of range of type long", str_to_conv);
    } else if (strtol_in > INT_MAX) {
        LOG_FATAL("%ld greater than INT_MAX", strtol_in);
    } else if (strtol_in < INT_MIN) {
        LOG_FATAL("%ld less than INT_MIN", strtol_in);
    } else {
        *store_into = (int)strtol_in;
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

/******************************************************
 *** EXPOSED METHODS                                ***
 ******************************************************/

CLIArguments* cli_config_default(void) {
    CLIArguments* conf = malloc(sizeof(CLIArguments));

    conf->logging_level = FATAL;
    conf->rom_path = "";

    return conf;
}

void cli_config_destroy(CLIArguments *const conf) {
    free(conf);
}

ErrorCode cli_config_handle(CLIArguments* const conf, const int argc, char **const argv) {
    int c;

    if (argc < 2) {
        LOG_FATAL("No ROM path specified!");
        fprintf(stderr, "%s\n", usage_str);
        return ERR_FAILURE;
    }

    // parse all options first
    int strtol_in;
    while ((c = getopt(argc, argv, "l:")) != -1) {
        switch (c) {
            case 'l':
                if (safe_strtol(optarg, &strtol_in))
                    return ERR_FAILURE;

                conf->logging_level = (LoggingLevel) strtol_in;
                log_set_lvl(conf->logging_level);
                break;
            default:
                fprintf(stderr, "%s\n", usage_str);
                return ERR_FAILURE;
        }
    }

    // parse the remaining options

    if (argc - optind > 1) {
        LOG_FATAL("You provided too many arguments!");
        fprintf(stderr, "%s\n", usage_str);
        return ERR_FAILURE;
    }

    if (optind >= argc) {
        LOG_FATAL("No ROM path specified!");
        fprintf(stderr, "%s\n", usage_str);
        return ERR_FAILURE;
    }

    // path to rom is the only remaining argument
    conf->rom_path = argv[optind++];
    LOG_DEBUG("Path to ROM: %s", conf->rom_path);

    return ERR_SUCCESS;
}

