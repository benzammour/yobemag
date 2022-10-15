#include <stdio.h>
#include <stdint.h>
#include "lcd.h"
#include "cpu.h"
#include "rom.h"
#include "mmu.h"
#include "cli.h"
#include "logging.h"

int main(const int argc, char **const argv) {
    CLIArguments* args = cli_config_default();
    
    if (cli_config_handle(args, argc, argv)) {
        cli_config_destroy(args);
        return -4;
    }

    uint8_t ret = rom_load("./roms/hello-world.gb");
    if (ret) {
        LOG_FATAL("Failed to load ROM");
        exit(-1);
    }
    LOG_INFO("Successfully initialized ROM");

    ret = mem_init();
    if (ret) {
        LOG_FATAL("Failed to initialize MMU");
        exit(-2);
    }
    LOG_INFO("Successfully initialized MMU");
    

    ret = lcd_load();
    if (ret) {
        LOG_FATAL("Failed to initialize LCD");
        exit(-3);
    }
    LOG_INFO("Successfully initialized LCD");


    cpu_init();
    LOG_INFO("Successfully initialized CPU");

    uint8_t x = 0;
    while(1) {
        if (cpu_step()) {
            break;
        }

        if (lcd_step()) {
            break;
        }

        x++;
    }
    LOG_INFO("Total number of interations: %d", x);

    SDL_Quit();
    cli_config_destroy(args);

    return 0;
}
