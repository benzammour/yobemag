#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"

typedef struct LD_8bit_TestParams {
    uint8_t opcode;
    uint8_t lhs_val;
    uint8_t rhs_val;
    uint8_t l_dword_reg;
    uint8_t l_word_offset;
    uint8_t r_dword_reg;
    uint8_t r_word_offset;
    uint8_t expected;
} LD_8bit_TestParams;

typedef struct LD_8bit_Special_TestParams {
    uint8_t opcode;
    uint8_t lhs_val;
    uint8_t rhs_val;
    uint8_t l_dword_reg;
    uint8_t l_word_offset;
    uint8_t expected;
    bool is_HL;
} LD_8bit_Special_TestParams;

void emulate_ld_instruction(LD_8bit_TestParams *params) {
    // setup cpu
    uint16_t address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    cpu.PC           = address;

    // setup cpu
    uint8_t *goal_reg   = get_cpu_reg(params->l_dword_reg, params->l_word_offset);
    uint8_t *source_reg = get_cpu_reg(params->r_dword_reg, params->r_word_offset);
    *goal_reg           = params->lhs_val;
    *source_reg         = params->rhs_val;

    mmu_write_byte(address, params->opcode);

    // do the actual emulation
    cpu_step();

    // check if value is correct
    uint8_t actual = *goal_reg;
    cr_expect(eq(u8, actual, params->expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

// LD B, n
ParameterizedTestParameters(ld_b_n, ld_b_n) {
    static LD_8bit_TestParams params[] = {
        {0x40, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.hi), 128},
        {0x41, 128, 1,   offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.lo), 1  },
        {0x42, 128, 2,   offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.hi), 2  },
        {0x43, 128, 3,   offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.lo), 3  },
        {0x44, 128, 4,   offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.hi), 4  },
        {0x45, 128, 5,   offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.lo), 5  },
        {0x47, 128, 6,   offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, AF),
         offsetof(DoubleWordReg, words.hi), 6  },
    };

    // generate parameter set
    return cr_make_param_array(LD_8bit_TestParams, params, sizeof(params) / sizeof(LD_8bit_TestParams));
}

ParameterizedTest(LD_8bit_TestParams *params, ld_b_n, ld_b_n, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_ld_instruction(params);
}

// LD B, HL
Test(ld_b_n, ld_b_hl_res_only, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    const uint8_t opcode        = 0x46;
    const uint8_t b             = 70;
    const uint8_t num           = 7;
    const uint8_t expected      = num;
    const uint16_t address      = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    const uint16_t word_address = (rand() + 0x8000) % (0x10000);

    // setup cpu
    cpu.PC      = address;
    CPU_REG_B   = b;
    CPU_DREG_HL = word_address;
    mmu_write_byte(address, opcode);
    mmu_write_byte(word_address, num);

    // do the actual emulation
    cpu_step();

    // check if value is correct
    const uint8_t actual = CPU_REG_B;
    cr_expect(eq(u8, actual, expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

// LD C, n
ParameterizedTestParameters(ld_c_n, ld_c_n) {
    static LD_8bit_TestParams params[] = {
        {0x48, 128, 1,   offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.hi), 1  },
        {0x49, 128, 128, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.lo), 128},
        {0x4A, 128, 2,   offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.hi), 2  },
        {0x4B, 128, 3,   offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.lo), 3  },
        {0x4C, 128, 4,   offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.hi), 4  },
        {0x4D, 128, 5,   offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.lo), 5  },
        {0x4F, 128, 6,   offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), offsetof(CPU, AF),
         offsetof(DoubleWordReg, words.hi), 6  },
    };

    // generate parameter set
    return cr_make_param_array(LD_8bit_TestParams, params, sizeof(params) / sizeof(LD_8bit_TestParams));
}

ParameterizedTest(LD_8bit_TestParams *params, ld_c_n, ld_c_n, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    emulate_ld_instruction(params);
}

// LD n, HL and LD n, d8
ParameterizedTestParameters(ld_c_n, ld_c_hl) {
    static LD_8bit_Special_TestParams params[] = {
        {0x46, 128, 12, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 12, true }, // LD B, HL
        {0x4E, 128, 12, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 12, true }, // LD C, HL

        {0x06, 128, 12, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 12, false}, // LD B, d8
        {0x0E, 128, 12, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 12, false}, // LD C, d8
    };

    return cr_make_param_array(LD_8bit_Special_TestParams, params,
                               sizeof(params) / sizeof(LD_8bit_Special_TestParams));
}

ParameterizedTest(LD_8bit_Special_TestParams *params, ld_c_n, ld_c_hl, .init = cpu_mmu_setup,
                  .fini = cpu_teardown) {
    uint8_t opcode             = params->opcode;
    uint8_t value              = params->rhs_val;
    uint16_t address_increment = 1;
    uint16_t address           = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    uint8_t *goal_reg          = get_cpu_reg(params->l_dword_reg, params->l_word_offset);

    // setup cpu
    cpu.PC    = address;
    *goal_reg = params->lhs_val;

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

    // check if value is correct
    cr_expect(eq(u8, *goal_reg, params->expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + address_increment));
}
