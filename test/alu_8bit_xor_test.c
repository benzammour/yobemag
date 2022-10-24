#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"
#include "common/alu.h"

ParameterizedTestParameters(XOR_A_n, XOR_A_n_same_destruction) {
    const uint8_t alternate    = 0b10101010;
    const uint8_t f_wanted     = 0x00 | (1 << Z_FLAG);
    static TestParams params[] = {
        {0xA8, alternate, alternate, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 0, 1, f_wanted},
        {0xA9, alternate, alternate, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 0, 1, f_wanted},
        {0xAA, alternate, alternate, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 0, 1, f_wanted},
        {0xAB, alternate, alternate, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 0, 1, f_wanted},
        {0xAC, alternate, alternate, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 0, 1, f_wanted},
        {0xAD, alternate, alternate, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 0, 1, f_wanted},
        {0xAF, alternate, alternate, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 0, 1, f_wanted},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, XOR_A_n, XOR_A_n_same_destruction, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_instruction(params);
}

ParameterizedTestParameters(XOR_A_n, XOR_A_HL_and_d8) {
    const uint8_t f_z_set             = 0x00 | (1 << Z_FLAG);
    static SpecialTestParams params[] = {
        {0xAE, 255, 255, 0,   f_z_set, true,  false},
        {0xAE, 8,   8,   0,   f_z_set, true,  false},
        {0xAE, 128, 128, 0,   f_z_set, true,  false},
        {0xAE, 254, 1,   255, 0,       true,  false},
        {0xAE, 0,   1,   1,   0,       true,  false},
        {0xAE, 128, 129, 1,   0,       true,  false},

        {0xEE, 255, 255, 0,   f_z_set, false, false},
        {0xEE, 8,   8,   0,   f_z_set, false, false},
        {0xEE, 128, 128, 0,   f_z_set, false, false},
        {0xEE, 254, 1,   255, 0,       false, false},
        {0xEE, 0,   1,   1,   0,       false, false},
        {0xEE, 128, 129, 1,   0,       false, false},
    };

    return cr_make_param_array(SpecialTestParams, params, sizeof(params) / sizeof(SpecialTestParams));
}

ParameterizedTest(SpecialTestParams *params, XOR_A_n, XOR_A_HL_and_d8, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_HL_d8_instruction(params);
}
