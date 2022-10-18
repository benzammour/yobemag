#include <criterion/criterion.h>
#include <stdio.h>
#include <time.h>

#include "cli.h"
#include "cpu.h"
#include "logging.h"
#include "mmu.h"

void cpu_setup(void) {
    srand(time(NULL));
    mmu_init();
    cpu_init();
}

void cpu_teardown(void) { mmu_destroy(); }

// Test SUB_A_A with no borrow
Test(simple, sub_a_a_no_borrow, .init = cpu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x97;
    const uint8_t a = 128;
    const uint8_t expected = 0;
    const uint16_t address = rand() % (0xFFFF);

    // setup cpu
    cpu.PC = address;
    cpu.AF.bytes.high = a;
    mmu_write_byte(address, opcode);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = cpu.AF.bytes.high;

    // Z should be set, as our result is 0
    cr_assert(cpu.AF.bytes.low & (1 << Z_FLAG));

    // N should be set
    cr_assert(cpu.AF.bytes.low & (1 << N_FLAG));

    // H should NOT be set
    cr_assert(!(cpu.AF.bytes.low & (1 << H_FLAG)));

    // C should NOT be set
    cr_assert(!(cpu.AF.bytes.low & (1 << C_FLAG)));

    // check if value is correct
    cr_assert(actual == expected);

    // check if pc is Pc is updated correctly
    cr_assert(cpu.PC == (address + 1));
}
