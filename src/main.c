#include <stdio.h>
#include <stdint.h>

#include "lcd.h"
#include "cpu.h"
#include "rom.h"
#include "mmu.h"
#include "cli.h"
#include "logging.h"
#include "err_codes.h"

int main(const int argc, char **const argv) {
    CLIArguments* cli_args = cli_config_handle(argc, argv);

	rom_init(cli_args->rom_path);
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

	fflush(stdout);
	fflush(stderr);
	SDL_Quit();
	rom_destroy();

    return EXIT_SUCCESS;
}
