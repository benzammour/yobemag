#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/logging.h>
#include <criterion/redirect.h>
#include <criterion/parameterized.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include "cli.h"

Test(cli, cli_no_args, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr) {
    char *argv[] = {"./yobemag"};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_wrong_option, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr) {
    char *argv[] = {"./yobemag", "-s", "0"};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_too_many_args, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr) {
    char *argv[] = {"./yobemag", "-l", "4", "../build/yobemag.gb", "test_param"};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_no_rom_path, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr) {
    char *argv[] = {"./yobemag", "-l", "4"};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_no_log_lvl_no_path, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr) {
    char *argv[] = {"./yobemag", "-l"};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_no_log_lvl, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr) {
    char *argv[] = {"./yobemag", "-l", "../build/yobemag.gb"};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_not_finished, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr) {
    char *argv[] = {"./yobemag", "-l", "1abc"};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_gt_than_long_max, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr) {
    char buf[50];
    snprintf(buf, 50, "%li9", LONG_MAX);

    char *argv[] = {"./yobemag", "-l", buf};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_lt_than_long_min, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr) {
    char buf[50];
    snprintf(buf, 50, "%li9", LONG_MIN);

    char *argv[] = {"./yobemag", "-l", buf};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_gt_than_int_max, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr) {
    char buf[50];
    snprintf(buf, 50, "%li9", LONG_MAX);

    char *argv[] = {"./yobemag", "-l", buf};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_lt_than_int_min, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr) {
    char buf[50];
    snprintf(buf, 50, "%li9", LONG_MIN);

    char *argv[] = {"./yobemag", "-l", buf};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);
}

Test(cli, cli_correct_with_log_lvl, .exit_code = EXIT_SUCCESS, .init = cr_redirect_stderr) {
    char *argv[] = {"./yobemag", "-l", "2", "../build/yobemag.gb"};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);

    LoggingLevel actual_log_lvl   = cli_args.logging_level;
    LoggingLevel expected_log_lvl = 2;

    char const *expected_rom_path = "../build/yobemag.gb";

    cr_expect(eq(int, actual_log_lvl, expected_log_lvl));
    cr_assert_str_eq(cli_args.rom_path, expected_rom_path);
}

Test(cli, cli_correct_no_log_lvl, .exit_code = EXIT_SUCCESS, .init = cr_redirect_stderr) {
    char *argv[] = {"./yobemag", "../build/yobemag.gb"};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);

    char const *expected_rom_path = "../build/yobemag.gb";

    cr_assert_str_eq(cli_args.rom_path, expected_rom_path);
}
