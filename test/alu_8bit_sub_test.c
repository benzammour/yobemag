#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "fixtures/cpu_mmu.h"

static uint8_t cpu_setup(const uint8_t opcode, const uint16_t address, const uint8_t lhs, const uint8_t rhs,
                         uint8_t *reg) {
    // setup cpu
    cpu.PC    = address;
    CPU_REG_A = lhs;
    *reg      = rhs;
    mmu_write_byte(address, opcode);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = CPU_REG_A;

    return actual;
}

Test(sub_a_n, sub_a_a_no_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x97;
    const uint16_t address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    const uint8_t a        = 128;
    const uint8_t expected = 0;

    uint8_t actual = cpu_setup(opcode, address, a, a, &CPU_REG_A);

    // Z should be set, as our result is 0
    cr_expect(eq(u8, get_flag(Z_FLAG), 1));

    // N should be set
    cr_expect(eq(u8, get_flag(N_FLAG), 1));

    // H should NOT be set because of no half-borrow
    cr_expect(zero(u8, get_flag(H_FLAG)));

    // C should NOT be set because of no borrow
    cr_expect(zero(u8, get_flag(C_FLAG)));

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, (address + 1)));
}

Test(sub_a_n, sub_a_b_only_half_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x90;
    const uint8_t a        = 0b0100101;
    const uint8_t b        = 0b0001101;
    const uint8_t expected = a - b;
    const uint16_t address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;

    uint8_t actual = cpu_setup(opcode, address, a, b, &CPU_REG_B);

    // Z should not be set, as our result is not 0
    cr_expect(zero(u8, get_flag(Z_FLAG)));

    // N should always be set
    cr_expect(eq(u8, get_flag(N_FLAG), 1));

    // H should be set because borrow occurred
    cr_expect(eq(u8, get_flag(H_FLAG), 1));

    // C should not be set because of no borrow
    cr_expect(zero(u8, get_flag(C_FLAG)));

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sub_a_n, sub_a_c_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x91;
    const uint8_t a        = 0b0000101;
    const uint8_t c        = 0b0010101;
    const uint8_t expected = a - c;
    const uint16_t address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;

    uint8_t actual = cpu_setup(opcode, address, a, c, &CPU_REG_C);

    // Z should not be set, as our result is not 0
    cr_expect(zero(u8, get_flag(Z_FLAG)));

    // N should always be set
    cr_expect(eq(u8, get_flag(N_FLAG), 1));

    // H should not be set because no HB occurred
    cr_expect(zero(u8, get_flag(H_FLAG)));

    // C should be set because of borrow
    cr_expect(eq(u8, get_flag(C_FLAG), 1));

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sub_a_n, sub_a_d_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x92;
    const uint8_t a        = random();
    const uint8_t d        = random();
    const uint8_t expected = a - d;
    const uint16_t address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;

    uint8_t actual = cpu_setup(opcode, address, a, d, &CPU_REG_D);

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sub_a_n, sub_a_e_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x93;
    const uint8_t a        = random();
    const uint8_t e        = random();
    const uint8_t expected = a - e;
    const uint16_t address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;

    uint8_t actual = cpu_setup(opcode, address, a, e, &CPU_REG_E);

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sub_a_n, sub_a_h_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x94;
    const uint8_t a        = random();
    const uint8_t h        = random();
    const uint8_t expected = a - h;
    const uint16_t address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;

    uint8_t actual = cpu_setup(opcode, address, a, h, &CPU_REG_H);

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sub_a_n, sub_a_l_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x95;
    const uint8_t a        = random();
    const uint8_t l        = random();
    const uint8_t expected = a - l;
    const uint16_t address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;

    uint8_t actual = cpu_setup(opcode, address, a, l, &CPU_REG_L);

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sub_a_n, sub_a_hl_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x96;
    const uint8_t a        = 70;
    const uint8_t num      = 1;
    const uint8_t expected = a - num;

    const uint16_t address      = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    const uint16_t word_address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;

    // setup cpu
    cpu.PC      = address;
    CPU_REG_A   = a;
    CPU_DREG_HL = word_address;
    mmu_write_byte(address, opcode);
    mmu_write_byte(word_address, num);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = CPU_REG_A;

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sub_a_n, sub_a_d8_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0xD6;
    const uint8_t a        = 70;
    const uint8_t num      = 1;
    const uint8_t expected = a - num;

    const uint16_t address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;

    // setup cpu
    cpu.PC    = address;
    CPU_REG_A = a;
    mmu_write_byte(address, opcode);
    mmu_write_byte(address + 1, num);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = CPU_REG_A;

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 2));
}
