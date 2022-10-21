#include "util.h"

uint8_t *get_cpu_regs(uint8_t reg_offset, uint8_t word_offset) {
    char *target_dword_reg = ((char *) &cpu) + reg_offset;
    uint8_t *target        = (uint8_t *) (target_dword_reg + word_offset);

    return target;
}
