#include <stdio.h>
#include <stdint.h>
#include "lcd.h"
#include "cpu.h"
#include "rom.h"
#include "mmu.h"

int main(void) {
    uint8_t ret = rom_load("./roms/hello-world.gb");
    if (ret) {
        fprintf(stderr, "Failed to load ROM\n");
        exit(-1);
    }
    printf("Successfully initialized ROM\n");

    ret = mem_init();
    if (ret) {
        fprintf(stderr, "Failed to initialize MMU\n");
        exit(-2);
    }
    printf("Successfully initialized MMU\n");
    

    ret = lcd_load();
    if (ret) {
        fprintf(stderr, "Failed to initialize LCD\n");
        exit(-3);
    }
    printf("Successfully initialized LCD\n");


    cpu_init();
    printf("Successfully initialized CPU\n");

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

    printf("%d\n", x);
    SDL_Quit();

    return 0;
}
