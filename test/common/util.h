#ifndef YOBEMAG_UTIL_H
#define YOBEMAG_UTIL_H

#include <stdint.h>
#include <stddef.h>
#include "cpu.h"

uint8_t *get_cpu_reg(size_t dreg_offset, size_t reg_offset);
uint16_t *get_cpu_dreg(size_t dreg_offset);

#endif // YOBEMAG_UTIL_H
