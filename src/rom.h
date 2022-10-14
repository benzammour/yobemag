#ifndef ROM_H
#define ROM_H
#include <stdint.h>

uint8_t rom_load(const char *filename);
__attribute__((pure)) uint8_t *get_rom_bytes(void);
uint8_t rom_setup(uint8_t *rom_data);

#endif
