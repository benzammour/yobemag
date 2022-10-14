#ifndef MEM_H
#define MEM_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


uint8_t mem_init(void);

__attribute__((pure)) uint8_t mmu_get_byte(uint16_t addr);
void mmu_write_byte(uint16_t dest_addr, uint8_t source_addr);
__attribute__((pure)) uint16_t mmu_get_two_bytes(uint16_t addr);
void mmu_write_two_bytes(uint16_t dest_addr, uint16_t value);

#endif
