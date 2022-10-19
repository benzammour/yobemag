#include "cpu_mmu.h"

void cpu_mmu_setup(void) {
    srand(time(NULL));
    cpu_init();
}

void cpu_teardown(void) { }
