#include "util.h"

uint8_t get_flag_bit(Flag f) {
    return (cpu.AF.bytes.low >> f) & 1;
}
