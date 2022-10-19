#include "cpu_mmu.h"

void cpu_mmu_setup(void) {
    srand(0xcafebeef);
    cpu_init();
}

void cpu_teardown(void) { }
