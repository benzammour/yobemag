#include <stdlib.h>
#include <limits.h>
#include <getopt.h>
#include <stdio.h>
#include <errno.h>

#include "cli.h"
#include "log.h"

/******************************************************
 *** LOCAL VARIABLES                                ***
 ******************************************************/

static char const *usage_str = "Usage: yobemag [-l <0..4>] <ROM>";

/******************************************************
 *** LOCAL METHODS                                  ***
 ******************************************************/

static void safe_strtol(char const *const str_to_conv, int *const store_into) {
    errno = 0;

    char *end;
    long const strtol_in = strtol(str_to_conv, &end, 10);

    if (end == str_to_conv) {
        YOBEMAG_EXIT("strtol failed: %s not a decimal number", str_to_conv);
    } else if ('\0' != *end) {
        YOBEMAG_EXIT("strtol failed: %s has extra characters at end of input: %s", str_to_conv, end);
    } else if ((LONG_MIN == strtol_in || LONG_MAX == strtol_in) && ERANGE == errno) {
        YOBEMAG_EXIT("strtol failed: %s out of range of type long", str_to_conv);
    } else if (strtol_in > INT_MAX) {
        YOBEMAG_EXIT("strtol failed: %ld greater than INT_MAX", strtol_in);
    } else if (strtol_in < INT_MIN) {
        YOBEMAG_EXIT("strtol failed: %ld less than INT_MIN", strtol_in);
    } else {
        *store_into = (int) strtol_in;
    }
}

/******************************************************
 *** EXPOSED METHODS                                ***
 ******************************************************/

void cli_parse(CLIArguments *const cli_args, int const argc, char **const argv) {
    if (argc < 2) {
        YOBEMAG_EXIT("No ROM path specified! %s", usage_str);
    }

    // set default values
    cli_args->logging_level = FATAL;

    // parse all options first
    int strtol_in;
    int c;
    while ((c = getopt(argc, argv, "l:")) != -1) {
        switch (c) {
            case 'l':
                safe_strtol(optarg, &strtol_in);
                cli_args->logging_level = (LoggingLevel) strtol_in;
                break;
            default:
                YOBEMAG_EXIT("%s", usage_str);
        }
    }

    // parse the remaining options
    if (argc - optind > 1) {
        YOBEMAG_EXIT("You provided too many arguments! %s", usage_str);
    } else if (optind >= argc) {
        YOBEMAG_EXIT("No ROM path specified! %s", usage_str);
    }

    // path to rom is the only remaining argument
    cli_args->rom_path = argv[optind++];
    LOG_DEBUG("Path to ROM: %s", cli_args->rom_path);
}
