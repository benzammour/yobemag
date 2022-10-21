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

Test(sbc_a_n, sbc_a_a_no_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x9F;
    const uint16_t address = (rand() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    const uint8_t a        = 128;
    const uint8_t expected = 0;
    set_flag(0, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, a, &CPU_REG_A);

    // Z should be set, as our result is 0
    cr_expect(eq(u8, get_flag_bit(Z_FLAG), 1));

    // N should be set
    cr_expect(eq(u8, get_flag_bit(N_FLAG), 1));

    // H should NOT be set because of no half-borrow
    cr_expect(zero(u8, get_flag_bit(H_FLAG)));

    // C should NOT be set because of no borrow
    cr_expect(zero(u8, get_flag_bit(C_FLAG)));

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, (address + 1)));
}

Test(sbc_a_n, sbc_a_b_only_half_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x98;
    const uint8_t a        = 0b0100111;
    const uint8_t b        = 0b0000111;
    const uint8_t expected = a - b - 1;
    const uint16_t address = (rand() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    set_flag(1, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, b, &CPU_REG_B);

    // Z should not be set, as our result is not 0
    cr_expect(zero(u8, get_flag_bit(Z_FLAG)));

    // N should always be set
    cr_expect(eq(u8, get_flag_bit(N_FLAG), 1));

    // H should be set because borrow occured
    cr_expect(eq(u8, get_flag_bit(H_FLAG), 1));

    // C should not be set because of no borrow
    cr_expect(zero(u8, get_flag_bit(C_FLAG)));

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sbc_a_n, sbc_a_c_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x99;
    const uint8_t a        = 0b0000111;
    const uint8_t c        = 0b0010101;
    const uint8_t expected = a - c - 1;
    const uint16_t address = (rand() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    set_flag(1, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, c, &CPU_REG_C);

    // Z should not be set, as our result is not 0
    cr_expect(zero(u8, get_flag_bit(Z_FLAG)));

    // N should always be set
    cr_expect(eq(u8, get_flag_bit(N_FLAG), 1));

    // H should not be set because no HB occured
    cr_expect(zero(u8, get_flag_bit(H_FLAG)));

    // C should be set because of borrow
    cr_expect(eq(u8, get_flag_bit(C_FLAG), 1));

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sbc_a_n, sbc_a_d_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x9A;
    const uint8_t a        = 200;
    const uint8_t d        = 101;
    const uint8_t expected = a - d - 1;
    const uint16_t address = (rand() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    set_flag(1, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, d, &CPU_REG_D);

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sbc_a_n, sbc_a_e_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x9B;
    const uint8_t a        = 200;
    const uint8_t e        = 101;
    const uint8_t expected = a - e - 1;
    const uint16_t address = (rand() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    set_flag(1, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, e, &CPU_REG_E);

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sbc_a_n, sbc_a_h_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x9C;
    const uint8_t a        = 200;
    const uint8_t h        = 101;
    const uint8_t expected = a - h - 1;
    const uint16_t address = (rand() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    set_flag(1, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, h, &CPU_REG_H);

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sbc_a_n, sbc_a_l_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x9D;
    const uint8_t a        = 200;
    const uint8_t l        = 101;
    const uint8_t expected = a - l - 1;
    const uint16_t address = (rand() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    set_flag(1, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, l, &CPU_REG_L);

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

Test(sbc_a_n, sbc_a_hl_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x9E;
    const uint8_t a        = 70;
    const uint8_t num      = 1;
    const uint8_t expected = a - num - 1;
    set_flag(1, C_FLAG);

    const uint16_t address      = (rand() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    const uint16_t word_address = (rand() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;

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

Test(sbc_a_n, sbc_a_d8_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0xDE;
    const uint8_t a        = 70;
    const uint8_t num      = 1;
    const uint8_t expected = a - num - 1;
    set_flag(1, C_FLAG);

    const uint16_t address = (rand() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;

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

Test(sbc_a_n, sbc_check_integer_promotion, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode   = 0x9D; // not important that it's exactly this one
    const uint8_t a        = 123;
    const uint8_t l        = 255;
    const uint8_t expected = (uint8_t) (a - l - 1);
    const uint16_t address = (rand() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    set_flag(1, C_FLAG);

    uint8_t actual = cpu_setup(opcode, address, a, l, &CPU_REG_L);

    // C should be set because of borrow
    // checks if integer promotion was successful
    cr_expect(eq(u8, get_flag_bit(C_FLAG), 1));

    // check if value is correct
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}
