#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/logging.h>
#include <criterion/redirect.h>
#include <signal.h>
#include <libgen.h>

#include "mmu.h"
#include "rom.h"
#include "log.h"

#define MAX_PATH_LENGTH    (512)
#define MAX_LOG_MSG_LENGTH (512)

Test(mmu, mmu_init_with_rom_load, .exit_code = EXIT_SUCCESS) {
    char *file_path_copy                = strdup(__FILE__);
    char rom_file_path[MAX_PATH_LENGTH] = {0};
    snprintf(rom_file_path, MAX_PATH_LENGTH, "%s/../roms/yobemag.gb", dirname(file_path_copy));
    rom_init(rom_file_path);
    mmu_init();
}

Test(mmu, mmu_get_byte_inside_rom, .exit_code = EXIT_SUCCESS) {
    cr_assert(mmu_get_byte(113) == 0x13);
}

Test(mmu, mmu_write_byte_to_rom, .init = cr_redirect_stderr) {
    log_set_lvl(ERROR);

    mmu_write_byte(0x100, 0xFF);

    char buf[MAX_LOG_MSG_LENGTH] = {0};
    FILE *f_stderr               = cr_get_redirected_stderr();
    while (fread(buf, 1, sizeof(buf), f_stderr) > 0) {};
    fclose(f_stderr);

    cr_assert_not_null(strstr(buf, "reserved"));
    cr_assert_not_null(strstr(buf, "0x100"));
    cr_assert_not_null(strstr(buf, "ERROR"));
}

Test(mmu, mmu_write_byte_outside_rom, .exit_code = EXIT_SUCCESS) {
    mmu_write_byte(ROM_LIMIT + 1, 0xFF);
    cr_assert(mmu_get_byte(ROM_LIMIT + 1) == 0xFF);
}

Test(mmu, mmu_write_two_bytes_to_rom, .init = cr_redirect_stderr) {
    log_set_lvl(ERROR);

    mmu_write_two_bytes(ROM_LIMIT - 1, 0xAB);

    char buf[MAX_LOG_MSG_LENGTH] = {0};
    FILE *f_stderr               = cr_get_redirected_stderr();
    while (fread(buf, 1, sizeof(buf), f_stderr) > 0) {};
    fclose(f_stderr);

    cr_assert_not_null(strstr(buf, "reserved"));
    cr_assert_not_null(strstr(buf, "0x7fff"));
    cr_assert_not_null(strstr(buf, "ERROR"));
}

Test(mmu, mmu_write_two_bytes_outside_rom, .exit_code = EXIT_SUCCESS) {
    mmu_write_two_bytes(ROM_LIMIT, 0xFF);
    cr_assert(mmu_get_two_bytes(ROM_LIMIT) == 0xFF);
}
