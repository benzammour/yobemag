#ifndef CPU_H
#define CPU_H
#include <stdint.h>

void cpu_init(void);
uint16_t cpu_get_cycle_count();
uint8_t cpu_step(void);

#endif
