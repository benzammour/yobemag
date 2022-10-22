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

    return cr_make_param_array(Ld8BitSpecialTestParams, params,
                               sizeof(params) / sizeof(Ld8BitSpecialTestParams));
}

ParameterizedTest(Ld8BitSpecialTestParams *params, ld_n_hl_d8, ld_n_hl_d8, .init = cpu_mmu_setup,
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
