#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

#include "fixtures/cpu_mmu.h"
#include "common/util.h"

typedef struct Ld8BitTestParams {
    uint8_t opcode;
    uint8_t lhs_val;
    uint8_t rhs_val;
    uint8_t l_dword_reg;
    uint8_t l_word_offset;
    uint8_t r_dword_reg;
    uint8_t r_word_offset;
    uint8_t expected;
} Ld8BitTestParams;

typedef struct Ld8BitSpecialTestParams {
    uint8_t opcode;
    uint8_t lhs_val;
    uint8_t rhs_val;
    uint8_t l_dword_reg;
    uint8_t l_word_offset;
    uint8_t expected;
    bool is_HL;
} Ld8BitSpecialTestParams;

typedef struct Ld8BitNNxTestParams {
    uint8_t opcode;
    uint8_t value;
    uint8_t dword_reg_offset;
    uint8_t expected;
} Ld8BitNNxTestParams;

/******************************************************
 *** LD m, n                                        ***
 ******************************************************/
ParameterizedTestParameters(ld_m_n, ld_m_n) {
    static Ld8BitTestParams params[] = {
  // LD B, n
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

 // LD C, n
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

 // LD D, n
        {0x50, 128, 1,   offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.hi), 1  },
        {0x51, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.lo), 128},
        {0x52, 128, 2,   offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.hi), 2  },
        {0x53, 128, 3,   offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.lo), 3  },
        {0x54, 128, 4,   offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.hi), 4  },
        {0x55, 128, 5,   offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.lo), 5  },
        {0x57, 128, 6,   offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), offsetof(CPU, AF),
         offsetof(DoubleWordReg, words.hi), 6  },

 // LD E, n
        {0x58, 128, 1,   offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.hi), 1  },
        {0x59, 128, 128, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.lo), 128},
        {0x5A, 128, 2,   offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.hi), 2  },
        {0x5B, 128, 3,   offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.lo), 3  },
        {0x5C, 128, 4,   offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.hi), 4  },
        {0x5D, 128, 5,   offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.lo), 5  },
        {0x5F, 128, 6,   offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), offsetof(CPU, AF),
         offsetof(DoubleWordReg, words.hi), 6  },

 // LD H, n
        {0x60, 128, 1,   offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.hi), 1  },
        {0x61, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.lo), 128},
        {0x62, 128, 2,   offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.hi), 2  },
        {0x63, 128, 3,   offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.lo), 3  },
        {0x64, 128, 4,   offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.hi), 4  },
        {0x65, 128, 5,   offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.lo), 5  },
        {0x67, 128, 6,   offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), offsetof(CPU, AF),
         offsetof(DoubleWordReg, words.hi), 6  },

 // LD L, n
        {0x68, 128, 1,   offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.hi), 1  },
        {0x69, 128, 128, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.lo), 128},
        {0x6A, 128, 2,   offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.hi), 2  },
        {0x6B, 128, 3,   offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.lo), 3  },
        {0x6C, 128, 4,   offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.hi), 4  },
        {0x6D, 128, 5,   offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.lo), 5  },
        {0x6F, 128, 6,   offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), offsetof(CPU, AF),
         offsetof(DoubleWordReg, words.hi), 6  },

 // LD A, n
        {0x78, 128, 1,   offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.hi), 1  },
        {0x79, 128, 128, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.lo), 128},
        {0x7A, 128, 2,   offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.hi), 2  },
        {0x7B, 128, 3,   offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.lo), 3  },
        {0x7C, 128, 4,   offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.hi), 4  },
        {0x7D, 128, 5,   offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.lo), 5  },
        {0x7F, 128, 6,   offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), offsetof(CPU, AF),
         offsetof(DoubleWordReg, words.hi), 6  },
    };

    // generate parameter set
    return cr_make_param_array(Ld8BitTestParams, params, sizeof(params) / sizeof(Ld8BitTestParams));
}

ParameterizedTest(Ld8BitTestParams *params, ld_m_n, ld_m_n, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    // setup cpu
    uint16_t address    = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    uint8_t *goal_reg   = get_cpu_reg(params->l_dword_reg, params->l_word_offset);
    uint8_t *source_reg = get_cpu_reg(params->r_dword_reg, params->r_word_offset);
    *goal_reg           = params->lhs_val;
    *source_reg         = params->rhs_val;
    cpu.PC              = address;

    mmu_write_byte(address, params->opcode);

    // do the actual emulation
    cpu_step();

    // check if value is correct
    uint8_t actual = *goal_reg;
    cr_expect(eq(u8, actual, params->expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + 1));
}

/******************************************************
 *** LD n, HL and LD n,d8                           ***
 ******************************************************/
ParameterizedTestParameters(ld_n_hl_d8, ld_n_hl_d8) {
    static Ld8BitSpecialTestParams params[] = {
        {0x46, 128, 12, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 12, true }, // LD B, HL
        {0x4E, 128, 12, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 12, true }, // LD C, HL
        {0x56, 128, 12, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 12, true }, // LD D, HL
        {0x5E, 128, 12, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 12, true }, // LD E, HL
        {0x66, 128, 12, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 12, true }, // LD D, HL
        {0x6E, 128, 12, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 12, true }, // LD E, HL
        {0x7E, 128, 12, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 12, true }, // LD A, HL

        {0x06, 128, 12, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), 12, false}, // LD B, d8
        {0x0E, 128, 12, offsetof(CPU, BC), offsetof(DoubleWordReg, words.lo), 12, false}, // LD C, d8
        {0x16, 128, 12, offsetof(CPU, DE), offsetof(DoubleWordReg, words.hi), 12, false}, // LD D, d8
        {0x1E, 128, 12, offsetof(CPU, DE), offsetof(DoubleWordReg, words.lo), 12, false}, // LD E, d8
        {0x26, 128, 12, offsetof(CPU, HL), offsetof(DoubleWordReg, words.hi), 12, false}, // LD H, d8
        {0x2E, 128, 12, offsetof(CPU, HL), offsetof(DoubleWordReg, words.lo), 12, false}, // LD L, d8
        {0x3E, 128, 12, offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 12, false}, // LD A, d8
    };

    return cr_make_param_array(Ld8BitSpecialTestParams, params, sizeof(params) / sizeof(Ld8BitSpecialTestParams));
}

ParameterizedTest(Ld8BitSpecialTestParams *params, ld_n_hl_d8, ld_n_hl_d8, .init = cpu_mmu_setup, .fini = cpu_teardown) {
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

/******************************************************
 *** LD HL, n                                       ***
 ******************************************************/
ParameterizedTestParameters(ld_hl_n, ld_hl_n) {
    static Ld8BitTestParams params[] = {
        {0x70, 128, 1, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.hi), 1},
        {0x71, 128, 2, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, BC),
         offsetof(DoubleWordReg, words.lo), 2},
        {0x72, 128, 3, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.hi), 3},
        {0x73, 128, 4, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, DE),
         offsetof(DoubleWordReg, words.lo), 4},
        {0x74, 128, 5, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.hi), 5},
        {0x75, 120, 6, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, HL),
         offsetof(DoubleWordReg, words.lo), 6},
        {0x77, 128, 7, offsetof(CPU, BC), offsetof(DoubleWordReg, words.hi), offsetof(CPU, AF),
         offsetof(DoubleWordReg, words.hi), 7},
    };

    // generate parameter set
    return cr_make_param_array(Ld8BitTestParams, params, sizeof(params) / sizeof(Ld8BitTestParams));
}

ParameterizedTest(Ld8BitTestParams *params, ld_hl_n, ld_hl_n, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    // setup cpu
    uint16_t opcode_addr     = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    uint16_t byte_write_addr = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    uint8_t *source_reg      = get_cpu_reg(params->r_dword_reg, params->r_word_offset);
    cpu.PC                   = opcode_addr;

    mmu_write_byte(opcode_addr, params->opcode);
    CPU_DREG_HL = byte_write_addr;
    *source_reg = params->rhs_val;

    // do the actual emulation
    cpu_step();

    // check if value is corr ect
    uint8_t actual = mmu_get_byte(CPU_DREG_HL);
    cr_expect(eq(u8, actual, params->expected), "ex: %d act: %d rhs: %d", params->expected, actual, params->rhs_val);

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, opcode_addr + 1));
}

/******************************************************
 *** LD HL, d8                                      ***
 ******************************************************/
ParameterizedTestParameters(ld_hl_d8, ld_hl_d8) {
    static Ld8BitNNxTestParams params[] = {
        {0x36, 12, offsetof(CPU, HL), 12}, // LD HL, d8
    };

    return cr_make_param_array(Ld8BitNNxTestParams, params, sizeof(params) / sizeof(Ld8BitNNxTestParams));
}

ParameterizedTest(Ld8BitNNxTestParams *params, ld_hl_d8, ld_hl_d8, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    uint8_t opcode               = params->opcode;
    uint8_t value                = params->value;
    uint16_t address_increment   = 2;
    uint16_t address             = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    uint16_t indirection_address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    uint16_t *goal_reg           = get_cpu_dreg(params->dword_reg_offset);

    // setup cpu
    cpu.PC    = address;
    *goal_reg = indirection_address;

    mmu_write_byte(address, opcode);
    mmu_write_byte(address + 1, value);

    // do the actual emulation
    cpu_step();

    // check if value is correct
    cr_expect(eq(u8, mmu_get_byte(indirection_address), params->expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + address_increment));
}

/******************************************************
 *** LD NN, x und LD x, NN                          ***
 ******************************************************/
typedef struct Ld8BitxNNTestParams {
    uint8_t opcode;
    uint8_t lhs_val;
    uint8_t rhs_val;
    uint8_t dword_reg_offset;
    uint8_t r_dword_reg;
    uint8_t r_word_offset;
    uint8_t expected;
    uint8_t type; // 0="normal", 1=HL++, 2=HL--
} Ld8BitxNNTestParams;

ParameterizedTestParameters(ld_nn_x, ld_nn_x) {
    static Ld8BitxNNTestParams params[] = {
        {0x02, 128, 1, offsetof(CPU, BC), offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 1, 0},
        {0x12, 128, 1, offsetof(CPU, DE), offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 1, 0},
        {0x22, 128, 1, offsetof(CPU, HL), offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 1, 1},
        {0x32, 128, 1, offsetof(CPU, HL), offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 1, 2},
    };

    // generate parameter set
    return cr_make_param_array(Ld8BitxNNTestParams, params, sizeof(params) / sizeof(Ld8BitxNNTestParams));
}

ParameterizedTest(Ld8BitxNNTestParams *params, ld_nn_x, ld_nn_x, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    // setup cpu
    uint16_t opcode_addr         = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    uint16_t indirection_address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    uint16_t *goal_reg           = get_cpu_dreg(params->dword_reg_offset);
    cpu.PC                       = opcode_addr;

    mmu_write_byte(opcode_addr, params->opcode);
    mmu_write_byte(indirection_address, params->lhs_val);

    CPU_REG_A       = params->rhs_val;
    *goal_reg       = indirection_address;
    uint16_t backup = *goal_reg;

    // do the actual emulation
    cpu_step();

    // check if value is correct
    uint8_t actual = mmu_get_byte(indirection_address);
    cr_expect(eq(u8, actual, params->expected), "ex: %d act: %d rhs: %d", params->expected, actual, params->rhs_val);

    // check if PC is updated correctly
    cr_expect(eq(u16, cpu.PC, opcode_addr + 1));

    // check if HL is updated correctly
    if (params->type == 1) {
        cr_expect(eq(u16, (backup + 1), CPU_DREG_HL));
    } else if (params->type == 2) {
        cr_expect(eq(u16, (backup - 1), CPU_DREG_HL));
    }
}

ParameterizedTestParameters(ld_nn_x, ld_x_nn) {
    static Ld8BitxNNTestParams params[] = {
        {0x0A, 128, 1, offsetof(CPU, BC), offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 1, 0},
        {0x1A, 128, 1, offsetof(CPU, DE), offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 1, 0},
        {0x2A, 128, 1, offsetof(CPU, HL), offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 1, 1},
        {0x3A, 128, 1, offsetof(CPU, HL), offsetof(CPU, AF), offsetof(DoubleWordReg, words.hi), 1, 2},
    };

    // generate parameter set
    return cr_make_param_array(Ld8BitxNNTestParams, params, sizeof(params) / sizeof(Ld8BitxNNTestParams));
}

ParameterizedTest(Ld8BitxNNTestParams *params, ld_nn_x, ld_x_nn, .init = cpu_mmu_setup, .fini = cpu_teardown) {
    uint8_t opcode               = params->opcode;
    uint8_t value                = params->rhs_val;
    uint16_t address_increment   = 1;
    uint16_t indirection_address = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;
    uint8_t *goal_reg            = get_cpu_reg(params->r_dword_reg, params->r_word_offset);
    uint16_t *source_reg         = get_cpu_dreg(params->dword_reg_offset);
    uint16_t address             = (random() % (MEM_SIZE - ROM_LIMIT)) + ROM_LIMIT;

    // setup cpu
    cpu.PC    = address;
    *goal_reg = params->lhs_val;

    mmu_write_byte(address, opcode);
    *source_reg = indirection_address;
    mmu_write_byte(indirection_address, value);
    uint16_t backup = *source_reg;

    // do the actual emulation
    cpu_step();

    // check if value is correct
    cr_expect(eq(u8, *goal_reg, params->expected));

    // check if PC is updated correctly
    cr_expect(eq(u8, cpu.PC, address + address_increment));

    // check if HL is updated correctly
    if (params->type == 1) {
        cr_expect(eq(u16, (backup + 1), CPU_DREG_HL));
    } else if (params->type == 2) {
        cr_expect(eq(u16, (backup - 1), CPU_DREG_HL));
    }
}
