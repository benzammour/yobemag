#include <stdio.h>
#include <stdint.h>

#include "lcd.h"
#include "cpu.h"
#include "rom.h"
#include "mmu.h"
#include "cli.h"
#include "log.h"

static void teardown(void) {
    fflush(stderr);
    fflush(stdout);
    // Both of these functions are "secured" against calling before initialization
    SDL_Quit();
    rom_destroy();
}

int main(int const argc, char **const argv) {
    // First thing we do is register exit hook
    atexit(&teardown);

    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);

    log_set_lvl(cli_args.logging_level);

    rom_init(cli_args.rom_path);
    LOG_INFO("Successfully initialized ROM");

    mmu_init();
    LOG_INFO("Successfully initialized MMU");

    lcd_init();
    LOG_INFO("Successfully initialized LCD");

    cpu_init();
    LOG_INFO("Successfully initialized CPU");

    uint8_t iterations = 0;
    while (1) {
        cpu_step();

        if (lcd_step())
            break;

        ++iterations;
    }
    LOG_INFO("Total number of iterations: %d", iterations);

    return EXIT_SUCCESS;
}
