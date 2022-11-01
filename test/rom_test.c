#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/logging.h>
#include <criterion/redirect.h>
#include <string.h>
#include <libgen.h>

#include "rom.h"
#include "log.h"

#define MAX_PATH_LENGTH    (512)
#define MAX_LOG_MSG_LENGTH (512)

Test(rom, rom_init_file_not_found, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr) {
    rom_init("some/weird/path/yobemag.gb");
}

Test(rom, rom_init_file_found, .exit_code = EXIT_SUCCESS, .init = cr_redirect_stderr) {
    char *file_path_copy                = strdup(__FILE__);
    char rom_file_path[MAX_PATH_LENGTH] = {0};
    snprintf(rom_file_path, MAX_PATH_LENGTH, "%s/../roms/yobemag.gb", dirname(file_path_copy));

    cr_expect_null(get_rom_bytes());
    rom_init(rom_file_path);
    cr_expect_not_null(get_rom_bytes());

    free(file_path_copy);
}

Test(rom, rom_check_destruction, .exit_code = EXIT_SUCCESS, .init = cr_redirect_stderr) {
    char *file_path_copy                = strdup(__FILE__);
    char rom_file_path[MAX_PATH_LENGTH] = {0};
    snprintf(rom_file_path, MAX_PATH_LENGTH, "%s/../roms/yobemag.gb", dirname(file_path_copy));

    rom_init(rom_file_path);
    rom_destroy();

    free(file_path_copy);
}

Test(rom, rom_invalid_destruction, .exit_code = EXIT_FAILURE, .init = cr_redirect_stderr) {
    log_set_lvl(FATAL);

    rom_destroy();

    char buf[MAX_LOG_MSG_LENGTH] = {0};
    FILE *f_stderr               = cr_get_redirected_stderr();
    while (fread(buf, 1, sizeof(buf), f_stderr) > 0) {};
    fclose(f_stderr);

    cr_expect_not_null(strstr(buf, "munmap failed"));
}

Test(log, rom_setup, .exit_code = EXIT_SUCCESS, .init = cr_redirect_stderr) {
    log_set_lvl(INFO);

    char *file_path_copy                = strdup(__FILE__);
    char rom_file_path[MAX_PATH_LENGTH] = {0};
    snprintf(rom_file_path, MAX_PATH_LENGTH, "%s/../roms/yobemag.gb", dirname(file_path_copy));
    rom_init(rom_file_path);

    char buf[MAX_LOG_MSG_LENGTH] = {0};
    FILE *f_stderr               = cr_get_redirected_stderr();
    while (fread(buf, 1, sizeof(buf), f_stderr) > 0) {};
    fclose(f_stderr);

    cr_expect_not_null(strstr(buf, "title"));
    cr_expect_not_null(strstr(buf, "type"));
    cr_expect_not_null(strstr(buf, "rom size"));

    free(file_path_copy);
}
