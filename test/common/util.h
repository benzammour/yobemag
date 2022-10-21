#ifndef YOBEMAG_UTIL_H
#define YOBEMAG_UTIL_H

#include <stdint.h>
#include "cpu.h"

uint8_t get_flag_bit(Flag f);
uint8_t* get_cpu_regs(uint8_t reg_offset, uint8_t word_offset);

#endif //YOBEMAG_UTIL_H
