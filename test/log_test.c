#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/logging.h>
#include <criterion/redirect.h>
#include <string.h>

#include "log.h"
#include "cli.h"

#define MAX_LOG_MSG_LENGTH (512)

Test(log, log_fatal_cli, .init = cr_redirect_stderr) {
    char *argv[] = {"./build/yobemag", "-l", "0"};
    int argc     = sizeof(argv) / sizeof(char *);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);

    FILE *f_stderr = cr_get_redirected_stderr();

    char buf[MAX_LOG_MSG_LENGTH] = {0};
    while (fread(buf, 1, sizeof(buf), f_stderr) > 0)
        ;
    buf[MAX_LOG_MSG_LENGTH - 1] = '\0';

    fclose(f_stderr);

    cr_assert(strstr(buf, "FATAL") != NULL);
}

Test(log, log_debug_cli, .init = cr_redirect_stderr) {
    char *argv[] = {"./build/yobemag", "-l", "0", "./yobemag.gb"};
    int argc     = sizeof(argv) / sizeof(char *);
    log_set_lvl(DEBUG);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);

    char buf[MAX_LOG_MSG_LENGTH] = {0};
    FILE *f_stderr               = cr_get_redirected_stderr();
    while (fread(buf, 1, sizeof(buf), f_stderr) > 0)
        ;
    fclose(f_stderr);

    cr_assert(strstr(buf, "DEBUG") != NULL);
}

Test(log, log_set_log_lvl, .init = cr_redirect_stderr) {
    log_set_lvl(INFO);

    char buf[MAX_LOG_MSG_LENGTH] = {0};
    FILE *f_stderr               = cr_get_redirected_stderr();
    while (fread(buf, 1, sizeof(buf), f_stderr) > 0)
        ;
    fclose(f_stderr);

    cr_assert(strstr(buf, "INFO") != NULL);
    cr_assert(strstr(buf, "initialized") != NULL);
}

Test(log, log_respect_log_lvl, .init = cr_redirect_stderr) {
    log_set_lvl(WARNING);

    LOG_DEBUG("Debug message");
    LOG_WARNING("Warning message");

    char buf[MAX_LOG_MSG_LENGTH] = {0};
    FILE *f_stderr               = cr_get_redirected_stderr();
    while (fread(buf, 1, sizeof(buf), f_stderr) > 0)
        ;
    fclose(f_stderr);

    cr_assert(strstr(buf, "DEBUG") == NULL);
    cr_assert(strstr(buf, "WARNING") != NULL);
}

Test(log, log_warn_about_clamping, .init = cr_redirect_stderr) {
    log_set_lvl(DEBUG - 1);
    log_set_lvl(FATAL + 1);

    char buf[MAX_LOG_MSG_LENGTH] = {0};
    FILE *f_stderr               = cr_get_redirected_stderr();
    while (fread(buf, 1, sizeof(buf), f_stderr) > 0)
        ;
    fclose(f_stderr);

    cr_assert(strstr(buf, "lower") != NULL);
    cr_assert(strstr(buf, "higher") != NULL);
}
