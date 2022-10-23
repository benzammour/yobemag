#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include <stdio.h>
#include <stdint.h>

#include "cli.h"

#include "common/util.h"

Test(cli, cli_no_args, .exit_code = EXIT_FAILURE) {
    char *argv[] = {"./yobemag"};
    int argc     = 1;

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_wrong_option, .exit_code = EXIT_FAILURE) {
    char *argv[] = {"./yobemag", "-s", "0"};
    int argc     = 3;

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_too_many_args, .exit_code = EXIT_FAILURE) {
    char *argv[] = {"./yobemag", "-l", "4", "../build/yobemag.gb", "test_param"};
    int argc     = 5;

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_no_rom_path, .exit_code = EXIT_FAILURE) {
    char *argv[] = {"./yobemag", "-l", "4"};
    int argc     = 3;

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_no_log_lvl_no_path, .exit_code = EXIT_FAILURE) {
    char *argv[] = {"./yobemag", "-l"};
    int argc     = 2;

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_no_log_lvl, .exit_code = EXIT_FAILURE) {
    char *argv[] = {"./yobemag", "-l", "../build/yobemag.gb"};
    int argc     = 3;

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_not_finished, .exit_code = EXIT_FAILURE) {
    char *argv[] = {"./yobemag", "-l", "1abc"};
    int argc     = 3;

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_gt_than_long_max, .exit_code = EXIT_FAILURE) {
    int const n = snprintf(NULL, 0, "%li", LONG_MAX);
    char buf[n + 2];
    int c  = snprintf(buf, n + 1, "%li", LONG_MAX);
    buf[n] = '9';

    char *argv[] = {"./yobemag", "-l", buf};
    int argc     = 3;

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_lt_than_long_min, .exit_code = EXIT_FAILURE) {
    int const n = snprintf(NULL, 0, "%li", LONG_MIN);
    char buf[n + 2];
    int c  = snprintf(buf, n + 1, "%li", LONG_MIN);
    buf[n] = '9';

    char *argv[] = {"./yobemag", "-l", buf};
    int argc     = 3;

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_gt_than_int_max, .exit_code = EXIT_FAILURE) {
    int const n = snprintf(NULL, 0, "%li", LONG_MAX);
    char buf[n + 2];
    int c = snprintf(buf, n + 1, "%li", LONG_MAX);

    char *argv[] = {"./yobemag", "-l", buf};
    int argc     = 3;

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_lt_than_int_min, .exit_code = EXIT_FAILURE) {
    int const n = snprintf(NULL, 0, "%li", LONG_MIN);
    char buf[n + 2];
    int c = snprintf(buf, n + 1, "%li", LONG_MIN);

    char *argv[] = {"./yobemag", "-l", buf};
    int argc     = 3;

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_correct_with_log_lvl, .exit_code = EXIT_SUCCESS) {
    char *argv[] = {"./yobemag", "-l", "2", "../build/yobemag.gb"};
    int argc     = 4;

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);

    uint8_t actual_log_lvl   = cli_args.logging_level;
    uint8_t expected_log_lvl = 2;

    char const *expected_rom_path = "../build/yobemag.gb";

    cr_expect(eq(u8, actual_log_lvl, expected_log_lvl));
    cr_assert_str_eq(cli_args.rom_path, expected_rom_path);
}

Test(cli, cli_correct_no_log_lvl, .exit_code = EXIT_SUCCESS) {
    char *argv[] = {"./yobemag", "../build/yobemag.gb"};
    int argc     = 2;

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);

    char const *expected_rom_path = "../build/yobemag.gb";

    cr_assert_str_eq(cli_args.rom_path, expected_rom_path);
}
