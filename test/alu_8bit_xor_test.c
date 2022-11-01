#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"
#include "common/alu.h"

ParameterizedTestParameters(XOR_A_n, XOR_A_n_same_destruction) {
    const uint8_t alternate       = 0b10101010;
    const uint8_t f_expected      = 0x00 | (1 << Z_FLAG);
    static ALUTestParams params[] = {
        {0xA8, alternate, alternate, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 0, 1, f_expected},
        {0xA9, alternate, alternate, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 0, 1, f_expected},
        {0xAA, alternate, alternate, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 0, 1, f_expected},
        {0xAB, alternate, alternate, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 0, 1, f_expected},
        {0xAC, alternate, alternate, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 0, 1, f_expected},
        {0xAD, alternate, alternate, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 0, 1, f_expected},
        {0xAF, alternate, alternate, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 0, 1, f_expected},
    };

    return cr_make_param_array(ALUTestParams, params, sizeof(params) / sizeof(ALUTestParams));
}

ParameterizedTest(ALUTestParams *params, XOR_A_n, XOR_A_n_same_destruction, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    alu_emulate_8bit_instruction(params);
}

ParameterizedTestParameters(XOR_A_n, XOR_A_HL_and_d8) {
    const uint8_t f_z_set                = 0x00 | (1 << Z_FLAG);
    static ALUSpecialTestParams params[] = {
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

    return cr_make_param_array(ALUSpecialTestParams, params, sizeof(params) / sizeof(ALUSpecialTestParams));
}

ParameterizedTest(ALUSpecialTestParams *params, XOR_A_n, XOR_A_HL_and_d8, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    alu_emulate_8bit_HL_d8_instruction(params);
}
