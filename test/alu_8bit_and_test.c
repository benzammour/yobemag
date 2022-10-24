#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"
#include "common/alu.h"

ParameterizedTestParameters(AND_A_n, AND_A_n_msb) {
    static TestParams params[] = {
        {0xA0, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 128, 1, 0b00100000},
        {0xA1, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 128, 1, 0b00100000},
        {0xA2, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 128, 1, 0b00100000},
        {0xA3, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 128, 1, 0b00100000},
        {0xA4, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 128, 1, 0b00100000},
        {0xA5, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 128, 1, 0b00100000},
        {0xA7, 128, 128, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 128, 1, 0b00100000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, AND_A_n, AND_A_n_msb, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_instruction(params);
}

ParameterizedTestParameters(AND_A_n, AND_A_n_alternating) {
    static TestParams params[] = {
        {0xA0, 0b10101010, 0b01010101, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 0,          1, 0b10100000},
        {0xA1, 0b10101010, 0b01010101, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 0,          1, 0b10100000},
        {0xA2, 0b10101010, 0b01010101, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 0,          1, 0b10100000},
        {0xA3, 0b10101010, 0b01010101, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 0,          1, 0b10100000},
        {0xA4, 0b10101010, 0b01010101, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 0,          1, 0b10100000},
        {0xA5, 0b10101010, 0b01010101, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 0,          1, 0b10100000},
        {0xA7, 0b10101010, 0b01010101, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 0b01010101, 1,
         0b00100000                                                                                                   },
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, AND_A_n, AND_A_n_alternating, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    emulate_instruction(params);
}

ParameterizedTestParameters(AND_A_n, AND_A_n_full) {
    static TestParams params[] = {
        {0xA0, 255, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00100000},
        {0xA1, 255, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 255, 1, 0b00100000},
        {0xA2, 255, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00100000},
        {0xA3, 255, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 255, 1, 0b00100000},
        {0xA4, 255, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00100000},
        {0xA5, 255, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 255, 1, 0b00100000},
        {0xA7, 255, 255, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00100000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, AND_A_n, AND_A_n_full, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_instruction(params);
}

ParameterizedTestParameters(AND_A_n, AND_A_HL_and_d8) {
    static SpecialTestParams params[] = {
        {0xA6, 255, 255, 255, 0b00100000, true,  false},
        {0xA6, 8,   8,   8,   0b00100000, true,  false},
        {0xA6, 128, 128, 128, 0b00100000, true,  false},
        {0xA6, 254, 1,   0,   0b10100000, true,  false},
        {0xA6, 0,   0,   0,   0b10100000, true,  false},
        {0xA6, 128, 129, 128, 0b00100000, true,  false},

        {0xE6, 255, 255, 255, 0b00100000, false, false},
        {0xE6, 8,   8,   8,   0b00100000, false, false},
        {0xE6, 128, 128, 128, 0b00100000, false, false},
        {0xE6, 254, 1,   0,   0b10100000, false, false},
        {0xE6, 0,   0,   0,   0b10100000, false, false},
        {0xE6, 128, 129, 128, 0b00100000, false, false},
    };

    return cr_make_param_array(SpecialTestParams, params, sizeof(params) / sizeof(SpecialTestParams));
}

ParameterizedTest(SpecialTestParams *params, AND_A_n, AND_A_HL_and_d8, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    emulate_HL_d8_instruction(params);
}
