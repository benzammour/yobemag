#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"
#include "common/alu.h"

ParameterizedTestParameters(ADD_HL_n, ADD_HL_n_carry) {
    static ALUTestParams params[] = {
        {0x09, 32768, 32768, offsetof(CPU, BC), 0, 0, 1, 0b00010000},
        {0x19, 32768, 32768, offsetof(CPU, DE), 0, 0, 1, 0b00010000},
        {0x29, 32768, 32768, offsetof(CPU, HL), 0, 0, 1, 0b00010000},
        {0x39, 32768, 32768, offsetof(CPU, SP), 0, 0, 1, 0b00010000},
    };

    return cr_make_param_array(ALUTestParams, params, sizeof(params) / sizeof(ALUTestParams));
}

ParameterizedTest(ALUTestParams *params, ADD_HL_n, ADD_HL_n_carry, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    alu_emulate_16bit_instruction(params);
}

ParameterizedTestParameters(ADD_HL_n, ADD_HL_n_half_carry) {
    static ALUTestParams params[] = {
        {0x09, 2048, 2048, offsetof(CPU, BC), 0, 4096, 1, 0b00100000},
        {0x19, 2048, 2048, offsetof(CPU, DE), 0, 4096, 1, 0b00100000},
        {0x29, 2048, 2048, offsetof(CPU, HL), 0, 4096, 1, 0b00100000},
        {0x39, 2048, 2048, offsetof(CPU, SP), 0, 4096, 1, 0b00100000},
    };

    return cr_make_param_array(ALUTestParams, params, sizeof(params) / sizeof(ALUTestParams));
}

ParameterizedTest(ALUTestParams *params, ADD_HL_n, ADD_HL_n_half_carry, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    alu_emulate_16bit_instruction(params);
}

ParameterizedTestParameters(ADD_HL_n, ADD_HL_n_half_carry_and_carry) {
    static ALUTestParams params[] = {
        {0x09, UINT16_MAX, UINT16_MAX, offsetof(CPU, BC), 0, UINT16_MAX - 1, 1, 0b00110000},
        {0x19, UINT16_MAX, UINT16_MAX, offsetof(CPU, DE), 0, UINT16_MAX - 1, 1, 0b00110000},
        {0x29, UINT16_MAX, UINT16_MAX, offsetof(CPU, HL), 0, UINT16_MAX - 1, 1, 0b00110000},
        {0x39, UINT16_MAX, UINT16_MAX, offsetof(CPU, SP), 0, UINT16_MAX - 1, 1, 0b00110000},
    };

    return cr_make_param_array(ALUTestParams, params, sizeof(params) / sizeof(ALUTestParams));
}

ParameterizedTest(ALUTestParams *params, ADD_HL_n, ADD_HL_n_half_carry_and_carry, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    alu_emulate_16bit_instruction(params);
}
