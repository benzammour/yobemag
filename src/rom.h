#ifndef ROM_H
#define ROM_H
#include <stdint.h>

uint8_t rom_load(const char *filename);
uint8_t *get_rom_bytes();

#endif
