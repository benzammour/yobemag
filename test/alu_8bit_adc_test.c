#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"
#include "common/alu.h"

ParameterizedTestParameters(ADC_A_n, ADC_A_n_carry_no_borrow) {
    static TestParams params[] = {
        {0x88, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 0, 1, 0b10010000},
        {0x89, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 0, 1, 0b10010000},
        {0x8A, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 0, 1, 0b10010000},
        {0x8B, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 0, 1, 0b10010000},
        {0x8C, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 0, 1, 0b10010000},
        {0x8D, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 0, 1, 0b10010000},
        {0x8F, 128, 128, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 0, 1, 0b10010000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, ADC_A_n, ADC_A_n_carry_no_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    // turn carry flag OFF
    clear_flag(C_FLAG);
    emulate_instruction(params);
}

ParameterizedTestParameters(ADC_A_n, ADC_A_n_carry_borrow) {
    static TestParams params[] = {
        {0x88, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 1, 1, 0b00010000},
        {0x89, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 1, 1, 0b00010000},
        {0x8A, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 1, 1, 0b00010000},
        {0x8B, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 1, 1, 0b00010000},
        {0x8C, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 1, 1, 0b00010000},
        {0x8D, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 1, 1, 0b00010000},
        {0x8F, 128, 128, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 1, 1, 0b00010000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, ADC_A_n, ADC_A_n_carry_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    // turn carry flag ON
    set_flag(1, C_FLAG);
    emulate_instruction(params);
}

ParameterizedTestParameters(ADC_A_n, ADC_A_n_half_carry_no_borrow) {
    static TestParams params[] = {
        {0x88, 8, 8, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 16, 1, 0b00100000},
        {0x89, 8, 8, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 16, 1, 0b00100000},
        {0x8A, 8, 8, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 16, 1, 0b00100000},
        {0x8B, 8, 8, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 16, 1, 0b00100000},
        {0x8C, 8, 8, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 16, 1, 0b00100000},
        {0x8D, 8, 8, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 16, 1, 0b00100000},
        {0x8F, 8, 8, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 16, 1, 0b00100000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, ADC_A_n, ADC_A_n_half_carry_no_borrow, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    // turn carry flag OFF
    clear_flag(C_FLAG);
    emulate_instruction(params);
}

ParameterizedTestParameters(ADC_A_n, ADC_A_n_half_carry_borrow) {
    static TestParams params[] = {
        {0x88, 8, 8, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 17, 1, 0b00100000},
        {0x89, 8, 8, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 17, 1, 0b00100000},
        {0x8A, 8, 8, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 17, 1, 0b00100000},
        {0x8B, 8, 8, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 17, 1, 0b00100000},
        {0x8C, 8, 8, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 17, 1, 0b00100000},
        {0x8D, 8, 8, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 17, 1, 0b00100000},
        {0x8F, 8, 8, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 17, 1, 0b00100000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, ADC_A_n, ADC_A_n_half_carry_borrow, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    // turn carry flag ON
    set_flag(1, C_FLAG);
    emulate_instruction(params);
}

ParameterizedTestParameters(ADC_A_n, ADC_A_n_half_carry_and_carry_borrow) {
    static TestParams params[] = {
        {0x88, 255, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00110000},
        {0x89, 255, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 255, 1, 0b00110000},
        {0x8A, 255, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00110000},
        {0x8B, 255, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 255, 1, 0b00110000},
        {0x8C, 255, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00110000},
        {0x8D, 255, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 255, 1, 0b00110000},
        {0x8F, 255, 255, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 255, 1, 0b00110000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, ADC_A_n, ADC_A_n_half_carry_and_carry_borrow, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    // turn carry flag ON
    set_flag(1, C_FLAG);
    emulate_instruction(params);
}

ParameterizedTestParameters(ADC_A_n, ADC_A_n_half_carry_and_carry_no_borrow) {
    static TestParams params[] = {
        {0x88, 255, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 254, 1, 0b00110000},
        {0x89, 255, 255, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 254, 1, 0b00110000},
        {0x8A, 255, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 254, 1, 0b00110000},
        {0x8B, 255, 255, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 254, 1, 0b00110000},
        {0x8C, 255, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 254, 1, 0b00110000},
        {0x8D, 255, 255, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 254, 1, 0b00110000},
        {0x8F, 255, 255, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 254, 1, 0b00110000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, ADC_A_n, ADC_A_n_half_carry_and_carry_no_borrow, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    // turn carry flag OFF
    clear_flag(C_FLAG);
    emulate_instruction(params);
}

ParameterizedTestParameters(ADC_A_n, ADC_A_HL_and_d8_borrow_and_no_borrow) {
    static SpecialTestParams params[] = {
        {0x8E, 255, 255, 255, 0b00110000, true,  true },
        {0x8E, 8,   8,   17,  0b00100000, true,  true },
        {0x8E, 128, 128, 1,   0b00010000, true,  true },
        {0x8E, 254, 1,   0,   0b10110000, true,  true },
        {0x8E, 0,   0,   1,   0b00000000, true,  true },
        {0x8E, 128, 129, 2,   0b00010000, true,  true },

        {0xCE, 255, 255, 255, 0b00110000, false, true },
        {0xCE, 8,   8,   17,  0b00100000, false, true },
        {0xCE, 128, 128, 1,   0b00010000, false, true },
        {0xCE, 254, 1,   0,   0b10110000, false, true },
        {0xCE, 0,   0,   1,   0b00000000, false, true },
        {0xCE, 128, 129, 2,   0b00010000, false, true },

        {0x8E, 255, 255, 254, 0b00110000, true,  false},
        {0x8E, 8,   8,   16,  0b00100000, true,  false},
        {0x8E, 128, 128, 0,   0b10010000, true,  false},
        {0x8E, 254, 1,   255, 0b00000000, true,  false},
        {0x8E, 0,   0,   0,   0b10000000, true,  false},
        {0x8E, 128, 129, 1,   0b00010000, true,  false},

        {0xCE, 255, 255, 254, 0b00110000, false, false},
        {0xCE, 8,   8,   16,  0b00100000, false, false},
        {0xCE, 128, 128, 0,   0b10010000, false, false},
        {0xCE, 254, 1,   255, 0b00000000, false, false},
        {0xCE, 0,   0,   0,   0b10000000, false, false},
        {0xCE, 128, 129, 1,   0b00010000, false, false},
    };

    return cr_make_param_array(SpecialTestParams, params, sizeof(params) / sizeof(SpecialTestParams));
}

ParameterizedTest(SpecialTestParams *params, ADC_A_n, ADC_A_HL_and_d8_borrow_and_no_borrow, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    if (params->uses_borrow) {
        set_flag(1, C_FLAG);
    } else {
        clear_flag(C_FLAG);
    }
    emulate_HL_d8_instruction(params);
}
