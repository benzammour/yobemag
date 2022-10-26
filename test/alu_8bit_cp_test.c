#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"
#include "common/alu.h"

ParameterizedTestParameters(CP_A_n, CP_A_n_carry) {
    static TestParams params[] = {
        {0xB8, 127, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 127, 1, 0b01010000},
        {0xB9, 127, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 127, 1, 0b01010000},
        {0xBA, 127, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 127, 1, 0b01010000},
        {0xBB, 127, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 127, 1, 0b01010000},
        {0xBC, 127, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 127, 1, 0b01010000},
        {0xBD, 127, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 127, 1, 0b01010000},
        {0xBF, 127, 128, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 128, 1, 0b11000000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, CP_A_n, CP_A_n_carry, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_instruction(params);
}

ParameterizedTestParameters(CP_A_n, CP_A_n_half_carry) {
    static TestParams params[] = {
        {0xB8, 7, 8, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 7, 1, 0b01110000},
        {0xB9, 7, 8, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 7, 1, 0b01110000},
        {0xBA, 7, 8, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 7, 1, 0b01110000},
        {0xBB, 7, 8, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 7, 1, 0b01110000},
        {0xBC, 7, 8, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 7, 1, 0b01110000},
        {0xBD, 7, 8, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 7, 1, 0b01110000},
        {0xBF, 7, 8, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 8, 1, 0b11000000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, CP_A_n, CP_A_n_half_carry, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_instruction(params);
}

ParameterizedTestParameters(CP_A_n, CP_A_n_half_carry_and_carry) {
    static TestParams params[] = {
        {0xB8, 254, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 254, 1, 0b01110000},
        {0xB9, 254, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 254, 1, 0b01110000},
        {0xBA, 254, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 254, 1, 0b01110000},
        {0xBB, 254, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 254, 1, 0b01110000},
        {0xBC, 254, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 254, 1, 0b01110000},
        {0xBD, 254, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 254, 1, 0b01110000},
        {0xBF, 254, 255, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 255, 1, 0b11000000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, CP_A_n, CP_A_n_half_carry_and_carry, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_instruction(params);
}

ParameterizedTestParameters(CP_A_n, CP_A_HL_and_d8) {
    static SpecialTestParams params[] = {
        {0xBE, 254, 255, 254, 0b01110000, true,  false},
        {0xBE, 7,   8,   7,   0b01110000, true,  false},
        {0xBE, 127, 127, 127, 0b11000000, true,  false},
        {0xBE, 254, 1,   254, 0b01000000, true,  false},
        {0xBE, 0,   0,   0,   0b11000000, true,  false},

        {0xFE, 254, 255, 254, 0b01110000, false, false},
        {0xFE, 7,   8,   7,   0b01110000, false, false},
        {0xFE, 127, 127, 127, 0b11000000, false, false},
        {0xFE, 254, 1,   254, 0b01000000, false, false},
        {0xFE, 0,   0,   0,   0b11000000, false, false},
    };

    return cr_make_param_array(SpecialTestParams, params, sizeof(params) / sizeof(SpecialTestParams));
}

ParameterizedTest(SpecialTestParams *params, CP_A_n, CP_A_HL_and_d8, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_HL_d8_instruction(params);
}
