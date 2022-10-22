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
    run_test(params);
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
    run_test(params);
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
    run_test(params);
}

ParameterizedTestParameters(ADD_A_n, ADC_A_HL_and_d8_borrow_and_no_borrow) {
    static SpecialTestParams params[] = {
        {0x86, 255, 255, 254, 0b00110000, true },
        {0x86, 8,   8,   16,  0b00100000, true },
        {0x86, 128, 128, 0,   0b10010000, true },
        {0x86, 254, 1,   255, 0b00000000, true },
        {0x86, 0,   0,   0,   0b10000000, true },
        {0x86, 128, 129, 1,   0b00010000, true },

        {0xC6, 255, 255, 254, 0b00110000, false},
        {0xC6, 8,   8,   16,  0b00100000, false},
        {0xC6, 128, 128, 0,   0b10010000, false},
        {0xC6, 254, 1,   255, 0b00000000, false},
        {0xC6, 0,   0,   0,   0b10000000, false},
        {0xC6, 128, 129, 1,   0b00010000, false},
    };

    return cr_make_param_array(SpecialTestParams, params, sizeof(params) / sizeof(SpecialTestParams));
}

ParameterizedTest(SpecialTestParams *params, ADD_A_n, ADC_A_HL_and_d8_borrow_and_no_borrow,
                  .init = cpu_mmu_setup, .fini = cpu_teardown) {
    uint8_t opcode             = params->opcode;
    uint8_t A                  = params->lhs;
    uint8_t value              = params->rhs;
    uint16_t address           = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    uint16_t address_increment = 1;

    // setup cpu
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
