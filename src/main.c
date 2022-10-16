#include <stdio.h>
#include <stdint.h>

#include "lcd.h"
#include "cpu.h"
#include "rom.h"
#include "mmu.h"
#include "errcodes.h"

int main(void) {
    ErrorCode ret = rom_load("./roms/hello-world.gb");
    if (ret) {
        fprintf(stderr, "Failed to load ROM\n");
        exit((int)ret);
    }
    printf("Successfully initialized ROM\n");

    ret = mem_init();
    if (ret) {
        fprintf(stderr, "Failed to initialize MMU\n");
        exit((int)ret);
    }
    printf("Successfully initialized MMU\n");


    ret = lcd_load();
    if (ret) {
        fprintf(stderr, "Failed to initialize LCD\n");
        exit((int)ret);
    }
    printf("Successfully initialized LCD\n");


    cpu_init();
    printf("Successfully initialized CPU\n");

    uint8_t iterations = 0;
    while (1) {
        if (cpu_step())
            break;

        if (lcd_step())
            break;

        ++iterations;
    }

    printf("%d\n", iterations);
    SDL_Quit();

    return 0;
}
