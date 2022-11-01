#include "cpu_mmu.h"

void cpu_mmu_setup(void) {
    srandom(0xcafebeef);
    cpu_init();
    clear_flag_register();
}

void cpu_teardown(void) {
}
