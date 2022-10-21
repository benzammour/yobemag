#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"

typedef struct TestParams {
    uint8_t opcode;
    uint8_t lhs;
    uint8_t rhs;
    size_t rhs_dreg_offset;
    size_t rhs_reg_offset;
    uint8_t expected;
    int address_increment;
    uint8_t F;
} TestParams;

typedef struct SpecialTestParams {
    uint8_t opcode;
    uint8_t lhs;
    uint8_t rhs;
    bool uses_borrow;
    uint8_t expected;
    uint8_t F;
    bool is_HL;
} SpecialTestParams;

static void run_test(TestParams const *const params) {
    // setup cpu
    uint16_t address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    cpu.PC           = address;
    CPU_REG_A        = params->lhs;

    // write rhs to desired register
    uint8_t *target_reg = get_cpu_reg(params->rhs_dreg_offset, params->rhs_reg_offset);
    *target_reg         = params->rhs;

    mmu_write_byte(address, params->opcode);

    // do the actual emulation
    cpu_step();

    // check flag register
    cr_expect(eq(u8, CPU_REG_F, params->F));

    // check if value is correct
    uint8_t actual = CPU_REG_A;
    cr_expect(eq(u8, actual, params->expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, (address + params->address_increment)));
}

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

ParameterizedTest(TestParams *params, ADC_A_n, ADC_A_n_carry_no_borrow, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    // turn carry flag OFF
    clear_flag(C_FLAG);
    run_test(params);
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

ParameterizedTest(TestParams *params, ADC_A_n, ADC_A_n_carry_borrow, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    // turn carry flag ON
    set_flag(1, C_FLAG);
    run_test(params);
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
    run_test(params);
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

ParameterizedTest(TestParams *params, ADC_A_n, ADC_A_n_half_carry_borrow, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    // turn carry flag ON
    set_flag(1, C_FLAG);
    run_test(params);
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
    run_test(params);
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
    run_test(params);
}

ParameterizedTestParameters(ADC_A_n, ADC_A_HL_and_d8_borrow_and_no_borrow) {
    static SpecialTestParams params[] = {
        {0x8E, 255, 255, true,  255, 0b00110000, true },
        {0x8E, 8,   8,   true,  17,  0b00100000, true },
        {0x8E, 128, 128, true,  1,   0b00010000, true },
        {0x8E, 254, 1,   true,  0,   0b10110000, true },
        {0x8E, 0,   0,   true,  1,   0b00000000, true },
        {0x8E, 128, 129, true,  2,   0b00010000, true },

        {0xCE, 255, 255, true,  255, 0b00110000, false},
        {0xCE, 8,   8,   true,  17,  0b00100000, false},
        {0xCE, 128, 128, true,  1,   0b00010000, false},
        {0xCE, 254, 1,   true,  0,   0b10110000, false},
        {0xCE, 0,   0,   true,  1,   0b00000000, false},
        {0xCE, 128, 129, true,  2,   0b00010000, false},

        {0x8E, 255, 255, false, 254, 0b00110000, true },
        {0x8E, 8,   8,   false, 16,  0b00100000, true },
        {0x8E, 128, 128, false, 0,   0b10010000, true },
        {0x8E, 254, 1,   false, 255, 0b00000000, true },
        {0x8E, 0,   0,   false, 0,   0b10000000, true },
        {0x8E, 128, 129, false, 1,   0b00010000, true },

        {0xCE, 255, 255, false, 254, 0b00110000, false},
        {0xCE, 8,   8,   false, 16,  0b00100000, false},
        {0xCE, 128, 128, false, 0,   0b10010000, false},
        {0xCE, 254, 1,   false, 255, 0b00000000, false},
        {0xCE, 0,   0,   false, 0,   0b10000000, false},
        {0xCE, 128, 129, false, 1,   0b00010000, false},
    };

    return cr_make_param_array(SpecialTestParams, params, sizeof(params) / sizeof(SpecialTestParams));
}

ParameterizedTest(SpecialTestParams *params, ADC_A_n, ADC_A_HL_and_d8_borrow_and_no_borrow,
                  .init = cpu_mmu_setup, .fini = cpu_teardown) {
    uint8_t opcode             = params->opcode;
    uint8_t A                  = params->lhs;
    uint8_t value              = params->rhs;
    uint16_t address           = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    uint16_t address_increment = 1;

    // setup cpu
    set_flag(params->uses_borrow, C_FLAG);
    cpu.PC    = address;
    CPU_REG_A = A;
    mmu_write_byte(address, opcode);
    if (params->is_HL) {
        uint16_t indirection_address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
        CPU_DREG_HL                  = indirection_address;
        mmu_write_byte(indirection_address, value);
    } else {
        mmu_write_byte(address + 1, value);
        ++address_increment;
    }

    // do the actual emulation
    cpu_step();

    // check flag register
    cr_expect(eq(u8, CPU_REG_F, params->F));

    // check if value is correct
    cr_expect(eq(u8, CPU_REG_A, params->expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + address_increment));
}
