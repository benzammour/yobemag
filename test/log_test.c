#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/logging.h>
#include <criterion/redirect.h>
#include <string.h>

#include "log.h"

#define MAX_LOG_MSG_LENGTH (512)

Test(log, log_set_log_lvl, .init = cr_redirect_stderr) {
    log_set_lvl(INFO);

    log_teardown();

    char buf[MAX_LOG_MSG_LENGTH] = {0};
    FILE *f_stderr               = cr_get_redirected_stderr();
    while (fread(buf, 1, sizeof(buf), f_stderr) > 0) {};
    fclose(f_stderr);

    cr_assert_not_null(strstr(buf, "INFO"));
    cr_assert_not_null(strstr(buf, "initialized"));
}

Test(log, log_respect_log_lvl, .init = cr_redirect_stderr) {
    log_set_lvl(WARNING);

    LOG_DEBUG("Debug message");
    LOG_WARNING("Warning message");

    log_teardown();

    char buf[MAX_LOG_MSG_LENGTH] = {0};
    FILE *f_stderr               = cr_get_redirected_stderr();
    while (fread(buf, 1, sizeof(buf), f_stderr) > 0) {};
    fclose(f_stderr);

    cr_assert_null(strstr(buf, "DEBUG"));
    cr_assert_not_null(strstr(buf, "WARNING"));
}

Test(log, log_warn_about_clamping, .init = cr_redirect_stderr) {
    log_set_lvl(DEBUG - 1);
    log_set_lvl(FATAL + 1);

    log_teardown();

    char buf[MAX_LOG_MSG_LENGTH] = {0};
    FILE *f_stderr               = cr_get_redirected_stderr();
    while (fread(buf, 1, sizeof(buf), f_stderr) > 0) {};
    fclose(f_stderr);

    cr_assert_not_null(strstr(buf, "lower"));
    cr_assert_not_null(strstr(buf, "higher"));
    cr_assert_not_null(strstr(buf, "WARNING"));
}
