#include <stdlib.h>
#include <limits.h>
#include <getopt.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

#include "cli.h"
#include "logging.h"

/******************************************************
 *** LOCAL VARIABLES                                ***
 ******************************************************/

static CLIArguments cli_args = {
		.logging_level = FATAL,
		.rom_path = "",
};

/******************************************************
 *** LOCAL METHODS                                  ***
 ******************************************************/

static void safe_strtol(const char* const str_to_conv, int* const store_into) {
	errno = 0;

	char* end;
	bool failed = true;
	const long strtol_in = strtol(str_to_conv, &end, 10);

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
        *store_into = (int) strtol_in;
		failed = false;
    }

	if (failed) {
		YOBEMAG_EXIT(ERR_STRTOL_FAILED);
	}
}

/******************************************************
 *** EXPOSED METHODS                                ***
 ******************************************************/

CLIArguments* cli_config_handle(const int argc, char **const argv) {
	if (argc < 2) {
		YOBEMAG_EXIT(ERR_NO_ROM_PATH);
	}

	// parse all options first
	int strtol_in;
	int c;
	while ((c = getopt(argc, argv, "l:")) != -1) {
        switch (c) {
            case 'l':
				safe_strtol(optarg, &strtol_in);
                cli_args.logging_level = (LoggingLevel) strtol_in;
                log_set_lvl(cli_args.logging_level);
                break;
            default:
				YOBEMAG_EXIT(ERR_WRONG_USAGE);
        }
    }

    // parse the remaining options

	if (argc - optind > 1) {
		YOBEMAG_EXIT(ERR_TOO_MANY_ARGS);
	} else if (optind >= argc) {
		YOBEMAG_EXIT(ERR_NO_ROM_PATH);
	}

    // path to rom is the only remaining argument
    cli_args.rom_path = argv[optind++];
    LOG_DEBUG("Path to ROM: %s", cli_args.rom_path);

	return &cli_args;
}
