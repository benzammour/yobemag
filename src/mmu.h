#ifndef YOBEMAG_MEM_H
#define YOBEMAG_MEM_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void mmu_init(void);
__attribute__((pure)) uint8_t mmu_get_byte(uint16_t addr);
void mmu_write_byte(uint16_t dest_addr, uint8_t source_addr);
__attribute__((pure)) uint16_t mmu_get_two_bytes(uint16_t addr);
void mmu_write_two_bytes(uint16_t dest_addr, uint16_t value);
void mmu_destroy(void);

#endif //YOBEMAG_MEM_H
