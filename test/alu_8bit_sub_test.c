#include <criterion/criterion.h>

#include "fixtures/cpu_mmu.h"
#include "logging.h"

Test(sub_a_n, sub_a_a_no_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
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

    // H should NOT be set because of no half-borrow
    cr_assert(!(cpu.AF.bytes.low & (1 << H_FLAG)));

    // C should NOT be set because of no borrow
    cr_assert(!(cpu.AF.bytes.low & (1 << C_FLAG)));

    // check if value is correct
    cr_assert(actual == expected);

    // check if pc is Pc is updated correctly
    cr_assert(cpu.PC == (address + 1));
}

Test(sub_a_n, sub_a_b_only_half_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x90;
    const uint8_t a = 0b0100101;
    const uint8_t b = 0b0001101;
    const uint8_t expected = a - b;
    const uint16_t address = rand() % (0xFFFF);

    // setup cpu
    cpu.PC = address;
    cpu.AF.bytes.high = a;
    cpu.BC.bytes.high = b;
    mmu_write_byte(address, opcode);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = cpu.AF.bytes.high;

    // Z should not be set, as our result is not 0
    cr_assert(!(cpu.AF.bytes.low & (1 << Z_FLAG)));

    // N should always be set
    cr_assert(cpu.AF.bytes.low & (1 << N_FLAG));

    // H should be set because borrow occured
    cr_assert(cpu.AF.bytes.low & (1 << H_FLAG));

    // C should not be set because of no borrow
    cr_assert(!(cpu.AF.bytes.low & (1 << C_FLAG)));

    // check if value is correct
    cr_assert(actual == expected);

    // check if pc is Pc is updated correctly
    cr_assert(cpu.PC == (address + 1));
}

Test(sub_a_n, sub_a_c_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x91;
    const uint8_t a = 0b0000101;
    const uint8_t c = 0b0010101;
    const uint8_t expected = a - c;
    const uint16_t address = rand() % (0xFFFF);

    // setup cpu
    cpu.PC = address;
    cpu.AF.bytes.high = a;
    cpu.BC.bytes.low = c;
    mmu_write_byte(address, opcode);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = cpu.AF.bytes.high;

    // Z should not be set, as our result is not 0
    cr_assert(!(cpu.AF.bytes.low & (1 << Z_FLAG)));

    // N should always be set
    cr_assert(cpu.AF.bytes.low & (1 << N_FLAG));

    // H should not be set because no HB occured
    cr_assert(!(cpu.AF.bytes.low & (1 << H_FLAG)));

    // C should be set because of borrow
    cr_assert(cpu.AF.bytes.low & (1 << C_FLAG));

    // check if value is correct
    cr_assert(actual == expected);

    // check if pc is Pc is updated correctly
    cr_assert(cpu.PC == (address + 1));
}

Test(sub_a_n, sub_a_d_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x92;
    const uint8_t a = rand() % (0xFFFF);
    const uint8_t d = rand() % (0xFFFF);
    const uint8_t expected = a - d;
    const uint16_t address = rand() % (0xFFFF);

    // setup cpu
    cpu.PC = address;
    cpu.AF.bytes.high = a;
    cpu.DE.bytes.high = d;
    mmu_write_byte(address, opcode);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = cpu.AF.bytes.high;

    // check if value is correct
    cr_assert(actual == expected);

    // check if pc is Pc is updated correctly
    cr_assert(cpu.PC == (address + 1));
}

Test(sub_a_n, sub_a_e_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x93;
    const uint8_t a = rand() % (0xFFFF);
    const uint8_t e = rand() % (0xFFFF);
    const uint8_t expected = a - e;
    const uint16_t address = rand() % (0xFFFF);

    // setup cpu
    cpu.PC = address;
    cpu.AF.bytes.high = a;
    cpu.DE.bytes.low = e;
    mmu_write_byte(address, opcode);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = cpu.AF.bytes.high;

    // check if value is correct
    cr_assert(actual == expected);

    // check if pc is Pc is updated correctly
    cr_assert(cpu.PC == (address + 1));
}

Test(sub_a_n, sub_a_h_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x94;
    const uint8_t a = rand() % (0xFFFF);
    const uint8_t h = rand() % (0xFFFF);
    const uint8_t expected = a - h;
    const uint16_t address = rand() % (0xFFFF);

    // setup cpu
    cpu.PC = address;
    cpu.AF.bytes.high = a;
    cpu.HL.bytes.high = h;
    mmu_write_byte(address, opcode);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = cpu.AF.bytes.high;

    // check if value is correct
    cr_assert(actual == expected);

    // check if pc is Pc is updated correctly
    cr_assert(cpu.PC == (address + 1));
}

Test(sub_a_n, sub_a_l_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x95;
    const uint8_t a = rand() % (0xFFFF);
    const uint8_t l = rand() % (0xFFFF);
    const uint8_t expected = a - l;
    const uint16_t address = rand() % (0xFFFF);

    // setup cpu
    cpu.PC = address;
    cpu.AF.bytes.high = a;
    cpu.HL.bytes.low = l;
    mmu_write_byte(address, opcode);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = cpu.AF.bytes.high;

    // check if value is correct
    cr_assert(actual == expected);

    // check if pc is Pc is updated correctly
    cr_assert(cpu.PC == (address + 1));
}

Test(sub_a_n, sub_a_hl_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0x96;
    const uint8_t a = 70;
    const uint8_t num = 1;
    const uint8_t expected = a - num;

    const uint16_t address = rand() % (0xFFFF);
    const uint16_t word_address = (rand() + 0x8000) % (0x10000);

    // setup cpu
    cpu.PC = address;
    cpu.AF.bytes.high = a;
    cpu.HL.word = word_address;
    mmu_write_byte(address, opcode);
    mmu_write_byte(word_address, num);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = cpu.AF.bytes.high;

    // check if value is correct
    cr_assert(actual == expected);

    // check if pc is Pc is updated correctly
    cr_assert(cpu.PC == (address + 1));
}

Test(sub_a_n, sub_a_d8_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode = 0xD6;
    const uint8_t a = 70;
    const uint8_t num = 1;
    const uint8_t expected = a - num;

    const uint16_t address = rand() % (0xFFFF - 1);

    // setup cpu
    cpu.PC = address;
    cpu.AF.bytes.high = a;
    mmu_write_byte(address, opcode);
    mmu_write_byte(address+1, num);

    // do the actual emulation
    cpu_step();
    const uint8_t actual = cpu.AF.bytes.high;

    // check if value is correct
    cr_assert(actual == expected);

    // check if pc is Pc is updated correctly
    cr_assert(cpu.PC == (address + 1));
}
