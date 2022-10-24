#include "util.h"

uint8_t *get_cpu_reg(size_t dreg_offset, size_t reg_offset) {
    char *target_dword_reg = ((char *) &cpu) + dreg_offset;
    uint8_t *target_reg    = (uint8_t *) (target_dword_reg + reg_offset);

    return target_reg;
}

uint16_t *get_cpu_dreg(size_t dreg_offset) {
    char *target_dword_reg = ((char *) &cpu) + dreg_offset;
    uint16_t *target_reg   = (uint16_t *) (target_dword_reg);

    return target_reg;
}
