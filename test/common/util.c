#include "util.h"

uint8_t get_flag_bit(Flag f) {
    return (CPU_REG_F >> f) & 1;
}
