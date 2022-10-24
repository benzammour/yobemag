#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"
#include "common/alu.h"

ParameterizedTestParameters(OR_A_n, OR_A_n_msb) {
    static TestParams params[] = {
        {0xB0, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 128, 1, 0b00000000},
        {0xB1, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 128, 1, 0b00000000},
        {0xB2, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 128, 1, 0b00000000},
        {0xB3, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 128, 1, 0b00000000},
        {0xB4, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 128, 1, 0b00000000},
        {0xB5, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 128, 1, 0b00000000},
        {0xB7, 128, 128, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 128, 1, 0b00000000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, OR_A_n, OR_A_n_msb, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_instruction(params);
}

ParameterizedTestParameters(OR_A_n, OR_A_n_alternating) {
    static TestParams params[] = {
        {0xB0, 0b10101010, 0b01010101, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 255,        1, 0b00000000},
        {0xB1, 0b10101010, 0b01010101, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 255,        1, 0b00000000},
        {0xB2, 0b10101010, 0b01010101, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 255,        1, 0b00000000},
        {0xB3, 0b10101010, 0b01010101, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 255,        1, 0b00000000},
        {0xB4, 0b10101010, 0b01010101, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 255,        1, 0b00000000},
        {0xB5, 0b10101010, 0b01010101, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 255,        1, 0b00000000},
        {0xB7, 0b10101010, 0b01010101, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 0b01010101, 1, 0b00000000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, OR_A_n, OR_A_n_alternating, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    emulate_instruction(params);
}

ParameterizedTestParameters(OR_A_n, OR_A_n_full) {
    static TestParams params[] = {
        {0xB0, 255, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00000000},
        {0xB1, 255, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 255, 1, 0b00000000},
        {0xB2, 255, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00000000},
        {0xB3, 255, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 255, 1, 0b00000000},
        {0xB4, 255, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00000000},
        {0xB5, 255, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 255, 1, 0b00000000},
        {0xB7, 255, 255, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00000000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, OR_A_n, OR_A_n_full, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_instruction(params);
}

ParameterizedTestParameters(OR_A_n, AND_A_HL_and_d8) {
    static SpecialTestParams params[] = {
        {0xB6, 255, 255, 255, 0b00000000, true,  false},
        {0xB6, 8,   8,   8,   0b00000000, true,  false},
        {0xB6, 128, 128, 128, 0b00000000, true,  false},
        {0xB6, 254, 1,   255, 0b00000000, true,  false},
        {0xB6, 0,   0,   0,   0b10000000, true,  false},
        {0xB6, 128, 129, 129, 0b00000000, true,  false},

        {0xF6, 255, 255, 255, 0b00000000, false, false},
        {0xF6, 8,   8,   8,   0b00000000, false, false},
        {0xF6, 128, 128, 128, 0b00000000, false, false},
        {0xF6, 254, 1,   255, 0b00000000, false, false},
        {0xF6, 0,   0,   0,   0b10000000, false, false},
        {0xF6, 128, 129, 129, 0b00000000, false, false},
    };

    return cr_make_param_array(SpecialTestParams, params, sizeof(params) / sizeof(SpecialTestParams));
}

ParameterizedTest(SpecialTestParams *params, OR_A_n, AND_A_HL_and_d8, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    emulate_HL_d8_instruction(params);
}
