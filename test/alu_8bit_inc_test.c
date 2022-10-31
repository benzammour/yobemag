#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"
#include "common/alu.h"

ParameterizedTestParameters(INC_n, INC_n) {
    static TestParams params[] = {
  // half carry
        {0x04, 0, 15, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 16, 1, 0b00100000},
        {0x0C, 0, 15, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 16, 1, 0b00100000},
        {0x14, 0, 15, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 16, 1, 0b00100000},
        {0x1C, 0, 15, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 16, 1, 0b00100000},
        {0x24, 0, 15, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 16, 1, 0b00100000},
        {0x2C, 0, 15, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 16, 1, 0b00100000},
        {0x3C, 0, 15, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 16, 1, 0b00100000},
    };

    return cr_make_param_array(TestParams, params, sizeof(params) / sizeof(TestParams));
}

ParameterizedTest(TestParams *params, INC_n, INC_n, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    clear_flag_register();

    // setup cpu
    uint16_t address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    cpu.PC           = address;

    // write righthand-side (rhs) to desired register
    uint8_t *target_reg = get_cpu_reg(params->rhs_dreg_offset, params->rhs_reg_offset);
    *target_reg         = params->rhs;

    mmu_write_byte(address, params->opcode);

    // do the actual emulation
    cpu_step();

    // flag register
    cr_expect(eq(u8, CPU_REG_F, params->F), "l: %d, r: %d, op: 0x%x", params->lhs, params->rhs, params->opcode);

    // check if value is correct
    uint8_t actual = *target_reg;
    cr_expect(eq(u8, actual, params->expected), "l: %d, r: %d, op: 0x%x", params->lhs, params->rhs, params->opcode);

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, (address + params->address_increment)));
}

ParameterizedTestParameters(INC_n, INC_n_HL) {
    static SpecialTestParams params[] = {
        {0x34, 0, 0,  1,  0b00000000, true, false},
        {0x34, 0, 15, 16, 0b00100000, true, false}, // half-carry
    };

    return cr_make_param_array(SpecialTestParams, params, sizeof(params) / sizeof(SpecialTestParams));
}

ParameterizedTest(SpecialTestParams *params, INC_n, INC_n_HL, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    clear_flag_register();

    uint8_t opcode             = params->opcode;
    uint8_t value              = params->rhs;
    uint16_t address           = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    uint16_t address_increment = 1;

    // setup cpu
    cpu.PC = address;
    mmu_write_byte(address, opcode);
    uint16_t indirection_address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    CPU_DREG_HL                  = indirection_address;
    mmu_write_byte(indirection_address, value);

    // do the actual emulation
    cpu_step();

    // check flag register
    cr_expect(eq(u8, CPU_REG_F, params->F), "l: %d, r: %d, op: 0x%x, uses_borrow: %d", params->lhs, params->rhs,
              params->opcode, params->uses_borrow);

    // check if value is correct
    uint8_t actual = mmu_get_byte(indirection_address);
    cr_expect(eq(u8, actual, params->expected), "l: %d, r: %d, op: 0x%x, uses_borrow: %d", params->lhs, params->rhs,
              params->opcode, params->uses_borrow);

    // check if PC is updated correctly
    cr_expect(eq(u16, cpu.PC, address + address_increment), "cpu.PC: %d, address: %d, address-increment: 0x%x", cpu.PC,
              address, address_increment);
}
