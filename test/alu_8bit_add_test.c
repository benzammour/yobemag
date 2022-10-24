#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"
#include "common/alu.h"

ParameterizedTestParameters(ADD_A_n, ADD_A_n_carry) {
    static TestParams params[] = {
        {0x80, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 0, 1, 0b10010000},
        {0x81, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 0, 1, 0b10010000},
        {0x82, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 0, 1, 0b10010000},
        {0x83, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 0, 1, 0b10010000},
        {0x84, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 0, 1, 0b10010000},
        {0x85, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 0, 1, 0b10010000},
        {0x87, 128, 128, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 0, 1, 0b10010000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, ADD_A_n, ADD_A_n_carry, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_instruction(params);
}

ParameterizedTestParameters(ADD_A_n, ADD_A_n_half_carry) {
    static TestParams params[] = {
        {0x80, 8, 8, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 16, 1, 0b00100000},
        {0x81, 8, 8, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 16, 1, 0b00100000},
        {0x82, 8, 8, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 16, 1, 0b00100000},
        {0x83, 8, 8, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 16, 1, 0b00100000},
        {0x84, 8, 8, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 16, 1, 0b00100000},
        {0x85, 8, 8, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 16, 1, 0b00100000},
        {0x87, 8, 8, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 16, 1, 0b00100000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, ADD_A_n, ADD_A_n_half_carry, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    emulate_instruction(params);
}

ParameterizedTestParameters(ADD_A_n, ADD_A_n_half_carry_and_carry) {
    static TestParams params[] = {
        {0x80, 255, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 254, 1, 0b00110000},
        {0x81, 255, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 254, 1, 0b00110000},
        {0x82, 255, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 254, 1, 0b00110000},
        {0x83, 255, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 254, 1, 0b00110000},
        {0x84, 255, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 254, 1, 0b00110000},
        {0x85, 255, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 254, 1, 0b00110000},
        {0x87, 255, 255, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 254, 1, 0b00110000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, ADD_A_n, ADD_A_n_half_carry_and_carry, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    emulate_instruction(params);
}

ParameterizedTestParameters(ADD_A_n, ADC_A_HL_and_d8_borrow_and_no_borrow) {
    static SpecialTestParams params[] = {
        {0x86, 255, 255, 254, 0b00110000, true,  false},
        {0x86, 8,   8,   16,  0b00100000, true,  false},
        {0x86, 128, 128, 0,   0b10010000, true,  false},
        {0x86, 254, 1,   255, 0b00000000, true,  false},
        {0x86, 0,   0,   0,   0b10000000, true,  false},
        {0x86, 128, 129, 1,   0b00010000, true,  false},

        {0xC6, 255, 255, 254, 0b00110000, false, false},
        {0xC6, 8,   8,   16,  0b00100000, false, false},
        {0xC6, 128, 128, 0,   0b10010000, false, false},
        {0xC6, 254, 1,   255, 0b00000000, false, false},
        {0xC6, 0,   0,   0,   0b10000000, false, false},
        {0xC6, 128, 129, 1,   0b00010000, false, false},
    };

    return cr_make_param_array(SpecialTestParams, params, sizeof(params) / sizeof(SpecialTestParams));
}

ParameterizedTest(SpecialTestParams *params, ADD_A_n, ADC_A_HL_and_d8_borrow_and_no_borrow,
                  .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_HL_d8_instruction(params);
}
