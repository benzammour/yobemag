#ifndef YOBEMAG_TEST_CPU_MMU_H
#define YOBEMAG_TEST_CPU_MMU_H

#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include "cpu.h"
#include "mmu.h"

void cpu_mmu_setup(void);
void cpu_teardown(void);

#endif // YOBEMAG_TEST_CPU_MMU_H
