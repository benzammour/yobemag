#ifndef YOBEMAG_ROM_H
#define YOBEMAG_ROM_H

#include <stdint.h>

#include "err_codes.h"

void rom_init(const char *file_name);
void rom_destroy(void);
__attribute__((pure)) uint8_t *get_rom_bytes(void);

#endif //YOBEMAG_ROM_H
