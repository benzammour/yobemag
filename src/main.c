#include <stdio.h>
#include <stdint.h>

#include "lcd.h"
#include "cpu.h"
#include "rom.h"
#include "mmu.h"
#include "cli.h"
#include "log.h"

int main(const int argc, char **const argv) {
    CLIArguments cli_args;
    cli_parse(&cli_args, argc, argv);

    log_set_lvl(cli_args.logging_level);
    atexit(log_teardown);

    rom_init(cli_args.rom_path);
    atexit(rom_destroy);
    LOG_INFO("Successfully initialized ROM");

    mmu_init();
    LOG_INFO("Successfully initialized MMU");

    lcd_init();
    atexit(lcd_teardown);
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

    exit(EXIT_SUCCESS);
}
