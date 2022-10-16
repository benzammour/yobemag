#include <stdio.h>
#include <stdint.h>

#include "lcd.h"
#include "cpu.h"
#include "rom.h"
#include "mmu.h"
#include "cli.h"
#include "logging.h"
#include "errcodes.h"

int main(const int argc, char **const argv) {
    CLIArguments* args = cli_config_default();
    
    if (cli_config_handle(args, argc, argv)) {
        cli_config_destroy(args);
        exit(ERR_FAILURE);
    }

    ErrorCode ret = rom_load("./roms/hello-world.gb");
    if (ret) {
        LOG_FATAL("Failed to load ROM");
        exit(ret);
    }
    printf("Successfully initialized ROM\n");

    ret = mem_init();
    if (ret) {
        fprintf(stderr, "Failed to initialize MMU\n");
        exit(ret);
    }
    LOG_INFO("Successfully initialized MMU");

    ret = lcd_load();
    if (ret) {
        LOG_FATAL("Failed to initialize LCD");
        exit(ret);
    }
    LOG_INFO("Successfully initialized LCD");


    cpu_init();
    LOG_INFO("Successfully initialized CPU");

    uint8_t iterations = 0;
    while (1) {
        if (cpu_step())
            break;

        if (lcd_step())
            break;

        ++iterations;
    }
    LOG_INFO("Total number of interations: %d", iterations);


    SDL_Quit();
    cli_config_destroy(args);

    return 0;
}
