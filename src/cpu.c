#include <stdio.h>

#include "mmu.h"
#include "cpu.h"
#include "log.h"

#define LO_NIBBLE_MASK (0x0F)
//#define HI_NIBBLE_MASK (0xF0)
#define BYTE_MASK      (0xFF)

typedef void (*op_function)(void);

// Function is used for instruction array initialization, not recognized by compiler
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#if defined(YOBEMAG_DEBUG)

static void UNKNOWN_OPCODE(void) {
    static unsigned err_count = 0;
    LOG_ERROR("Unallowed/Unimplemented OP Code: 0x%x", cpu.opcode);
    if (++err_count > 100) {
        YOBEMAG_EXIT("Exceeded number of unallowed OP codes!");
    }
}

#else

_Noreturn static void UNKNOWN_OPCODE(void) {
    YOBEMAG_EXIT("An unsupported instruction was encountered: 0x%x", cpu.opcode);
}

#endif // defined(YOBEMAG_EXIT)

#pragma GCC diagnostic pop

static op_function instr_lookup[0xFF + 1]       = {[0 ... 0xFF] = &UNKNOWN_OPCODE};
static op_function cb_prefixed_lookup[0xFF + 1] = {[0 ... 0xFF] = &UNKNOWN_OPCODE};

CPU cpu = {
    .HL.dword = 0,
    .DE.dword = 0,
    .BC.dword = 0,
    .AF.dword = 0,
    0,
    0,
    0,
    0,
};

__attribute__((always_inline)) inline static void clear_flag_register(void) {
    CPU_REG_F = 0;
}

__attribute__((always_inline)) inline static void LD_REG_REG(uint8_t *register_one, uint8_t register_two) {
    *register_one = register_two;
}

static void LD_REG_d8(uint8_t *reg_addr) {
    uint8_t immediate = mmu_get_byte(cpu.PC + 1);
    LD_REG_REG(reg_addr, immediate);
}

void REG_XOR(uint8_t *register_x, uint8_t register_y) {
    *register_x ^= register_y;

    clear_flag_register();
    set_flag(!(*register_x), Z_FLAG);

    cpu.cycle_count += 1;
}

void REG_DEC(uint8_t *reg) {
    (*reg)--;

    // Flag C is not affected
    set_flag((*reg & 0xF) == 0xF, H_FLAG);
    set_flag(1, N_FLAG);
    set_flag(!(*reg), Z_FLAG);

    // TODO: This is 4 in most and 12 in one case,
    //  	 add where this function is called
    cpu.cycle_count += 1;
}

void REG_INC(uint8_t *reg) {
    (*reg)++;

    set_flag((*reg & 0xF) == 0, H_FLAG);
    set_flag(1, N_FLAG);
    set_flag(!(*reg), Z_FLAG);

    // TODO: This is 4 in most and 12 in one case,
    //  	 add where this function is called
    cpu.cycle_count += 1;
}

static void optable_init(void) {
    // Set up lookup table
    instr_lookup[0x00] = OPC_NOP;
    instr_lookup[0x01] = OPC_LD_BC;
    instr_lookup[0x02] = OPC_LD_BC_A;
    instr_lookup[0x03] = OPC_INC_BC;
    instr_lookup[0x04] = OPC_INC_B;
    instr_lookup[0x05] = OPC_DEC_B;
    instr_lookup[0x0D] = OPC_DEC_C;
    instr_lookup[0x12] = OPC_LD_DE_A;
    instr_lookup[0x15] = OPC_DEC_D;
    instr_lookup[0x1D] = OPC_DEC_E;
    // TODO: 0x20
    // TODO: 0x21
    instr_lookup[0x22] = OPC_LD_HL_PLUS_A;
    instr_lookup[0x25] = OPC_DEC_H;
    instr_lookup[0x2D] = OPC_DEC_L;
    instr_lookup[0x31] = OPC_LD_SP;
    instr_lookup[0x32] = OPC_LD_HL_MINUS_A;

    // 8-bit loads
    instr_lookup[0x40] = OPC_LD_B_B;
    instr_lookup[0x41] = OPC_LD_B_C;
    instr_lookup[0x42] = OPC_LD_B_D;
    instr_lookup[0x43] = OPC_LD_B_E;
    instr_lookup[0x44] = OPC_LD_B_H;
    instr_lookup[0x45] = OPC_LD_B_L;
    instr_lookup[0x46] = OPC_LD_B_HL;
    instr_lookup[0x47] = OPC_LD_B_A;
    instr_lookup[0x06] = OPC_LD_B_d8;

    instr_lookup[0x48] = OPC_LD_C_B;
    instr_lookup[0x49] = OPC_LD_C_C;
    instr_lookup[0x4A] = OPC_LD_C_D;
    instr_lookup[0x4B] = OPC_LD_C_E;
    instr_lookup[0x4C] = OPC_LD_C_H;
    instr_lookup[0x4D] = OPC_LD_C_L;
    instr_lookup[0x4E] = OPC_LD_C_HL;
    instr_lookup[0x4F] = OPC_LD_C_A;
    instr_lookup[0x0E] = OPC_LD_C_d8;

    instr_lookup[0x50] = OPC_LD_D_B;
    instr_lookup[0x51] = OPC_LD_D_C;
    instr_lookup[0x52] = OPC_LD_D_D;
    instr_lookup[0x53] = OPC_LD_D_E;
    instr_lookup[0x54] = OPC_LD_D_H;
    instr_lookup[0x55] = OPC_LD_D_L;
    instr_lookup[0x56] = OPC_LD_D_HL;
    instr_lookup[0x57] = OPC_LD_D_A;
    instr_lookup[0x16] = OPC_LD_D_d8;

    instr_lookup[0x58] = OPC_LD_E_B;
    instr_lookup[0x59] = OPC_LD_E_C;
    instr_lookup[0x5A] = OPC_LD_E_D;
    instr_lookup[0x5B] = OPC_LD_E_E;
    instr_lookup[0x5C] = OPC_LD_E_H;
    instr_lookup[0x5D] = OPC_LD_E_L;
    instr_lookup[0x5E] = OPC_LD_E_HL;
    instr_lookup[0x5F] = OPC_LD_E_A;
    instr_lookup[0x1E] = OPC_LD_E_d8;

    instr_lookup[0x60] = OPC_LD_H_B;
    instr_lookup[0x61] = OPC_LD_H_C;
    instr_lookup[0x62] = OPC_LD_H_D;
    instr_lookup[0x63] = OPC_LD_H_E;
    instr_lookup[0x64] = OPC_LD_H_H;
    instr_lookup[0x65] = OPC_LD_H_L;
    instr_lookup[0x66] = OPC_LD_H_HL;
    instr_lookup[0x67] = OPC_LD_H_A;
    instr_lookup[0x26] = OPC_LD_H_d8;

    instr_lookup[0x68] = OPC_LD_L_B;
    instr_lookup[0x69] = OPC_LD_L_C;
    instr_lookup[0x6A] = OPC_LD_L_D;
    instr_lookup[0x6B] = OPC_LD_L_E;
    instr_lookup[0x6C] = OPC_LD_L_H;
    instr_lookup[0x6D] = OPC_LD_L_L;
    instr_lookup[0x6E] = OPC_LD_L_HL;
    instr_lookup[0x6F] = OPC_LD_L_A;
    instr_lookup[0x2E] = OPC_LD_L_d8;

    instr_lookup[0x70] = OPC_LD_HL_B;
    instr_lookup[0x71] = OPC_LD_HL_C;
    instr_lookup[0x72] = OPC_LD_HL_D;
    instr_lookup[0x73] = OPC_LD_HL_E;
    instr_lookup[0x74] = OPC_LD_HL_H;
    instr_lookup[0x75] = OPC_LD_HL_L;
    instr_lookup[0x77] = OPC_LD_HL_A;
    instr_lookup[0x36] = OPC_LD_HL_d8;

    instr_lookup[0x78] = OPC_LD_A_B;
    instr_lookup[0x79] = OPC_LD_A_C;
    instr_lookup[0x7A] = OPC_LD_A_D;
    instr_lookup[0x7B] = OPC_LD_A_E;
    instr_lookup[0x7C] = OPC_LD_A_H;
    instr_lookup[0x7D] = OPC_LD_A_L;
    instr_lookup[0x7E] = OPC_LD_A_HL;
    instr_lookup[0x7F] = OPC_LD_A_A;
    instr_lookup[0x3E] = OPC_LD_A_d8;

    // 8-bit ALU: ADD A,n
    instr_lookup[0x80] = OPC_ADD_A_B;
    instr_lookup[0x81] = OPC_ADD_A_C;
    instr_lookup[0x82] = OPC_ADD_A_D;
    instr_lookup[0x83] = OPC_ADD_A_E;
    instr_lookup[0x84] = OPC_ADD_A_H;
    instr_lookup[0x85] = OPC_ADD_A_L;
    instr_lookup[0x86] = OPC_ADD_A_HL;
    instr_lookup[0x87] = OPC_ADD_A_A;
    instr_lookup[0xC6] = OPC_ADD_A_d8;

    // 8-bit ALU: ADC A,n
    instr_lookup[0x88] = OPC_ADC_A_B;
    instr_lookup[0x89] = OPC_ADC_A_C;
    instr_lookup[0x8A] = OPC_ADC_A_D;
    instr_lookup[0x8B] = OPC_ADC_A_E;
    instr_lookup[0x8C] = OPC_ADC_A_H;
    instr_lookup[0x8D] = OPC_ADC_A_L;
    instr_lookup[0x8E] = OPC_ADC_A_HL;
    instr_lookup[0x8F] = OPC_ADC_A_A;
    instr_lookup[0xCE] = OPC_ADC_A_d8;

    // 8-bit ALU: SUB A,n
    instr_lookup[0x90] = OPC_SUB_A_B;
    instr_lookup[0x91] = OPC_SUB_A_C;
    instr_lookup[0x92] = OPC_SUB_A_D;
    instr_lookup[0x93] = OPC_SUB_A_E;
    instr_lookup[0x94] = OPC_SUB_A_H;
    instr_lookup[0x95] = OPC_SUB_A_L;
    instr_lookup[0x96] = OPC_SUB_A_HL;
    instr_lookup[0x97] = OPC_SUB_A_A;
    instr_lookup[0xD6] = OPC_SUB_A_d8;

    // 8-bit ALU: SUB A,n
    instr_lookup[0x98] = OPC_SBC_A_B;
    instr_lookup[0x99] = OPC_SBC_A_C;
    instr_lookup[0x9A] = OPC_SBC_A_D;
    instr_lookup[0x9B] = OPC_SBC_A_E;
    instr_lookup[0x9C] = OPC_SBC_A_H;
    instr_lookup[0x9D] = OPC_SBC_A_L;
    instr_lookup[0x9E] = OPC_SBC_A_HL;
    instr_lookup[0x9F] = OPC_SBC_A_A;
    instr_lookup[0xDE] = OPC_SBC_A_d8;

    // 8-bit ALU: AND A,n
    instr_lookup[0xA0] = OPC_AND_A_B;
    instr_lookup[0xA1] = OPC_AND_A_C;
    instr_lookup[0xA2] = OPC_AND_A_D;
    instr_lookup[0xA3] = OPC_AND_A_E;
    instr_lookup[0xA4] = OPC_AND_A_H;
    instr_lookup[0xA5] = OPC_AND_A_L;
    instr_lookup[0xA6] = OPC_AND_A_HL;
    instr_lookup[0xA7] = OPC_AND_A_A;
    instr_lookup[0xE6] = OPC_AND_A_d8;

    // 8-bit ALU: OR A,n
    instr_lookup[0xB0] = OPC_OR_A_B;
    instr_lookup[0xB1] = OPC_OR_A_C;
    instr_lookup[0xB2] = OPC_OR_A_D;
    instr_lookup[0xB3] = OPC_OR_A_E;
    instr_lookup[0xB4] = OPC_OR_A_H;
    instr_lookup[0xB5] = OPC_OR_A_L;
    instr_lookup[0xB6] = OPC_OR_A_HL;
    instr_lookup[0xB7] = OPC_OR_A_A;
    instr_lookup[0xF6] = OPC_OR_A_d8;

    // 8-bit ALU: XOR A,n
    instr_lookup[0xA8] = OPC_XOR_A_B;
    instr_lookup[0xA9] = OPC_XOR_A_C;
    instr_lookup[0xAA] = OPC_XOR_A_D;
    instr_lookup[0xAB] = OPC_XOR_A_E;
    instr_lookup[0xAC] = OPC_XOR_A_H;
    instr_lookup[0xAD] = OPC_XOR_A_L;
    instr_lookup[0xAE] = OPC_XOR_A_HL;
    instr_lookup[0xAF] = OPC_XOR_A_A;
    instr_lookup[0xEE] = OPC_XOR_A_d8;

    // TODO: 0xC3

    cb_prefixed_lookup[0x0] = UNKNOWN_OPCODE;
}

/* ------------------ CPU Funcs */
void cpu_init(void) {
    optable_init();

    CPU_DREG_AF     = 0x01B0;
    CPU_DREG_BC     = 0x0013;
    CPU_DREG_DE     = 0x00D8;
    CPU_DREG_HL     = 0x014D;
    cpu.SP          = 0xFFFE;
    cpu.PC          = 0x0;
    cpu.cycle_count = 0;
}

void cpu_print_registers(void) {
    LOG_DEBUG("PC: %04X AF: %02X%02X, BC: %02X%02X, DE: %02X%02X, HL: %02X%02X, SP: %04X, cycles: %d", cpu.PC, CPU_REG_A,
              CPU_REG_F, CPU_REG_B, CPU_REG_C, CPU_REG_D, CPU_REG_E, CPU_REG_H, CPU_REG_L, cpu.SP, cpu.cycle_count);
}

void cpu_step(void) {
    cpu.opcode = mmu_get_byte(cpu.PC);

    LOG_DEBUG("%04X: (%02X %02X %02X) A: %02X B: %02X C: %02X D: %02X E: %02X", cpu.PC, cpu.opcode,
              mmu_get_byte(cpu.PC + 1), mmu_get_byte(cpu.PC + 2), CPU_REG_A, CPU_REG_B, CPU_REG_C, CPU_REG_D, CPU_REG_E);

    // Get and Execute c.opcode
    (*(instr_lookup[cpu.opcode]))();

    // We cannot know (here) the exact number of increments that the PC and cycle count need,
    // hence the instructions themselves do it
}

// OP-Codes
void OPC_NOP(void) {
    ++cpu.cycle_count;
    ++cpu.PC;
}

void OPC_LD_BC(void) {
    uint16_t i  = mmu_get_two_bytes(cpu.PC);
    CPU_DREG_BC = i;

    cpu.PC += 2;
    cpu.cycle_count += 3;
}

void OPC_LD_BC_A(void) {
    mmu_write_byte(CPU_DREG_BC, CPU_REG_A);
    ++cpu.cycle_count;
}

void OPC_INC_BC(void) {
    ++CPU_DREG_BC;
    cpu.cycle_count += 2;
}

void OPC_INC_B(void) {
    REG_INC(&CPU_REG_B);
}

void OPC_DEC_B(void) {
    REG_DEC(&CPU_REG_B);
}

void OPC_DEC_C(void) {
    REG_DEC(&CPU_REG_C);
}

void OPC_LD_DE_A(void) {
    mmu_write_byte(CPU_DREG_DE, CPU_REG_A);
    ++cpu.cycle_count;
}

void OPC_DEC_D(void) {
    REG_DEC(&CPU_REG_D);
}

void OPC_DEC_E(void) {
    REG_DEC(&CPU_REG_E);
}

void OPC_LD_HL_PLUS_A(void) {
    uint16_t i = CPU_DREG_HL;
    mmu_write_byte(i, CPU_REG_A);
    CPU_DREG_HL = i + 1;
    ++cpu.cycle_count;
}

void OPC_DEC_H(void) {
    REG_DEC(&CPU_REG_H);
}

void OPC_DEC_L(void) {
    REG_DEC(&CPU_REG_L);
}

void OPC_LD_SP(void) {
    cpu.SP = mmu_get_byte(cpu.PC);
    cpu.PC += 2;
    cpu.cycle_count += 3;
}

void OPC_LD_HL_MINUS_A(void) {
    ++cpu.cycle_count;
    uint16_t i = CPU_DREG_HL;
    mmu_write_byte(i, CPU_REG_A);
    CPU_DREG_HL = i - 1;
    ++cpu.cycle_count;
}

/******************************************************
 *** 8-BIT Loads                                    ***
 ******************************************************/
// LD B, n
void OPC_LD_B_B(void) {
    LD_REG_REG(&CPU_REG_B, CPU_REG_B);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_B_C(void) {
    LD_REG_REG(&CPU_REG_B, CPU_REG_C);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_B_D(void) {
    LD_REG_REG(&CPU_REG_B, CPU_REG_D);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_B_E(void) {
    LD_REG_REG(&CPU_REG_B, CPU_REG_E);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_B_H(void) {
    LD_REG_REG(&CPU_REG_B, CPU_REG_H);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_B_L(void) {
    LD_REG_REG(&CPU_REG_B, CPU_REG_L);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_B_HL(void) {
    LD_REG_REG(&CPU_REG_B, mmu_get_byte(CPU_DREG_HL));

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_B_A(void) {
    LD_REG_REG(&CPU_REG_B, CPU_REG_A);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_B_d8(void) {
    LD_REG_d8(&CPU_REG_B);

    cpu.PC += 2;
    cpu.cycle_count += 8;
}

// LD C, n
void OPC_LD_C_B(void) {
    LD_REG_REG(&CPU_REG_C, CPU_REG_B);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_C_C(void) {
    LD_REG_REG(&CPU_REG_C, CPU_REG_C);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_C_D(void) {
    LD_REG_REG(&CPU_REG_C, CPU_REG_D);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_C_E(void) {
    LD_REG_REG(&CPU_REG_C, CPU_REG_E);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_C_H(void) {
    LD_REG_REG(&CPU_REG_C, CPU_REG_H);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_C_L(void) {
    LD_REG_REG(&CPU_REG_C, CPU_REG_L);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_C_HL(void) {
    LD_REG_REG(&CPU_REG_C, mmu_get_byte(CPU_DREG_HL));

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_C_A(void) {
    LD_REG_REG(&CPU_REG_C, CPU_REG_A);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_C_d8(void) {
    LD_REG_d8(&CPU_REG_C);

    cpu.PC += 2;
    cpu.cycle_count += 8;
}

// LD D, n
void OPC_LD_D_B(void) {
    LD_REG_REG(&CPU_REG_D, CPU_REG_B);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_D_C(void) {
    LD_REG_REG(&CPU_REG_D, CPU_REG_C);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_D_D(void) {
    LD_REG_REG(&CPU_REG_D, CPU_REG_D);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_D_E(void) {
    LD_REG_REG(&CPU_REG_D, CPU_REG_E);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_D_H(void) {
    LD_REG_REG(&CPU_REG_D, CPU_REG_H);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_D_L(void) {
    LD_REG_REG(&CPU_REG_D, CPU_REG_L);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_D_HL(void) {
    LD_REG_REG(&CPU_REG_D, mmu_get_byte(CPU_DREG_HL));

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_D_A(void) {
    LD_REG_REG(&CPU_REG_D, CPU_REG_A);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_D_d8(void) {
    LD_REG_d8(&CPU_REG_D);

    cpu.PC += 2;
    cpu.cycle_count += 8;
}

// LD E, n
void OPC_LD_E_B(void) {
    LD_REG_REG(&CPU_REG_E, CPU_REG_B);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_E_C(void) {
    LD_REG_REG(&CPU_REG_E, CPU_REG_C);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_E_D(void) {
    LD_REG_REG(&CPU_REG_E, CPU_REG_D);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_E_E(void) {
    LD_REG_REG(&CPU_REG_E, CPU_REG_E);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_E_H(void) {
    LD_REG_REG(&CPU_REG_E, CPU_REG_H);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_E_L(void) {
    LD_REG_REG(&CPU_REG_E, CPU_REG_L);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_E_HL(void) {
    LD_REG_REG(&CPU_REG_E, mmu_get_byte(CPU_DREG_HL));

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_E_A(void) {
    LD_REG_REG(&CPU_REG_E, CPU_REG_A);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_E_d8(void) {
    LD_REG_d8(&CPU_REG_E);

    cpu.PC += 2;
    cpu.cycle_count += 8;
}

// LD H, n
void OPC_LD_H_B(void) {
    LD_REG_REG(&CPU_REG_H, CPU_REG_B);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_H_C(void) {
    LD_REG_REG(&CPU_REG_H, CPU_REG_C);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_H_D(void) {
    LD_REG_REG(&CPU_REG_H, CPU_REG_D);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_H_E(void) {
    LD_REG_REG(&CPU_REG_H, CPU_REG_E);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_H_H(void) {
    LD_REG_REG(&CPU_REG_H, CPU_REG_H);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_H_L(void) {
    LD_REG_REG(&CPU_REG_H, CPU_REG_L);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_H_HL(void) {
    LD_REG_REG(&CPU_REG_H, mmu_get_byte(CPU_DREG_HL));

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_H_A(void) {
    LD_REG_REG(&CPU_REG_H, CPU_REG_A);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_H_d8(void) {
    LD_REG_d8(&CPU_REG_H);

    cpu.PC += 2;
    cpu.cycle_count += 8;
}

// LD L, n
void OPC_LD_L_B(void) {
    LD_REG_REG(&CPU_REG_L, CPU_REG_B);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_L_C(void) {
    LD_REG_REG(&CPU_REG_L, CPU_REG_C);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_L_D(void) {
    LD_REG_REG(&CPU_REG_L, CPU_REG_D);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_L_E(void) {
    LD_REG_REG(&CPU_REG_L, CPU_REG_E);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_L_H(void) {
    LD_REG_REG(&CPU_REG_L, CPU_REG_H);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_L_L(void) {
    LD_REG_REG(&CPU_REG_L, CPU_REG_L);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_L_HL(void) {
    LD_REG_REG(&CPU_REG_L, mmu_get_byte(CPU_DREG_HL));

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_L_A(void) {
    LD_REG_REG(&CPU_REG_L, CPU_REG_A);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_L_d8(void) {
    LD_REG_d8(&CPU_REG_L);

    cpu.PC += 2;
    cpu.cycle_count += 8;
}

// LD HL, n
void OPC_LD_HL_B(void) {
    mmu_write_byte(CPU_DREG_HL, CPU_REG_B);

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_HL_C(void) {
    mmu_write_byte(CPU_DREG_HL, CPU_REG_C);

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_HL_D(void) {
    mmu_write_byte(CPU_DREG_HL, CPU_REG_D);

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_HL_E(void) {
    mmu_write_byte(CPU_DREG_HL, CPU_REG_E);

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_HL_H(void) {
    mmu_write_byte(CPU_DREG_HL, CPU_REG_H);

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_HL_L(void) {
    mmu_write_byte(CPU_DREG_HL, CPU_REG_L);

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_HL_A(void) {
    mmu_write_byte(CPU_DREG_HL, CPU_REG_A);

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_HL_d8(void) {
    uint8_t immediate = mmu_get_byte(cpu.PC + 1);
    mmu_write_byte(CPU_DREG_HL, immediate);

    cpu.PC += 2;
    cpu.cycle_count += 12;
}

// LD A, n
void OPC_LD_A_B(void) {
    LD_REG_REG(&CPU_REG_A, CPU_REG_B);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_A_C(void) {
    LD_REG_REG(&CPU_REG_A, CPU_REG_C);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_A_D(void) {
    LD_REG_REG(&CPU_REG_A, CPU_REG_D);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_A_E(void) {
    LD_REG_REG(&CPU_REG_A, CPU_REG_E);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_A_H(void) {
    LD_REG_REG(&CPU_REG_A, CPU_REG_H);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_A_L(void) {
    LD_REG_REG(&CPU_REG_A, CPU_REG_L);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_A_HL(void) {
    LD_REG_REG(&CPU_REG_A, mmu_get_byte(CPU_DREG_HL));

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_LD_A_A(void) {
    LD_REG_REG(&CPU_REG_A, CPU_REG_A);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_LD_A_d8(void) {
    LD_REG_d8(&CPU_REG_A);

    cpu.PC += 2;
    cpu.cycle_count += 8;
}

/******************************************************
 *** 8-BIT ALU                                      ***
 ******************************************************/
static void ADD_A_n(uint8_t n) {
    uint8_t A            = CPU_REG_A;
    uint8_t A_nibble     = A & LO_NIBBLE_MASK;
    uint8_t n_nibble     = n & LO_NIBBLE_MASK;
    uint_fast16_t result = A + n;

    clear_flag_register(); // N is implicitly cleared
    set_flag(result > BYTE_MASK, C_FLAG);
    set_flag(A_nibble + n_nibble > LO_NIBBLE_MASK, H_FLAG);
    set_flag((result &= BYTE_MASK) == 0, Z_FLAG);

    CPU_REG_A = (uint8_t) result;
}

void OPC_ADD_A_A(void) {
    ADD_A_n(CPU_REG_A);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADD_A_B(void) {
    ADD_A_n(CPU_REG_B);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADD_A_C(void) {
    ADD_A_n(CPU_REG_C);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADD_A_D(void) {
    ADD_A_n(CPU_REG_D);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADD_A_E(void) {
    ADD_A_n(CPU_REG_E);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADD_A_H(void) {
    ADD_A_n(CPU_REG_H);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADD_A_L(void) {
    ADD_A_n(CPU_REG_L);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADD_A_HL(void) {
    ADD_A_n(mmu_get_byte(CPU_DREG_HL));
    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_ADD_A_d8(void) {
    uint8_t immediate = mmu_get_byte(cpu.PC + 1);
    ADD_A_n(immediate);
    cpu.cycle_count += 8;
    cpu.PC += 2;
}

static void ADC_A_n(uint8_t n) {
    uint8_t A            = CPU_REG_A;
    uint8_t A_nibble     = A & LO_NIBBLE_MASK;
    uint8_t n_nibble     = n & LO_NIBBLE_MASK;
    uint8_t c_flag       = get_flag(C_FLAG);
    uint_fast16_t result = (uint_fast16_t) (A + n + c_flag);

    clear_flag_register(); // N is implicitly cleared
    set_flag(result > BYTE_MASK, C_FLAG);
    set_flag(A_nibble + n_nibble + c_flag > LO_NIBBLE_MASK, H_FLAG);
    set_flag((result &= BYTE_MASK) == 0, Z_FLAG);

    CPU_REG_A = (uint8_t) result;
}

void OPC_ADC_A_A(void) {
    ADC_A_n(CPU_REG_A);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADC_A_B(void) {
    ADC_A_n(CPU_REG_B);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADC_A_C(void) {
    ADC_A_n(CPU_REG_C);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADC_A_D(void) {
    ADC_A_n(CPU_REG_D);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADC_A_E(void) {
    ADC_A_n(CPU_REG_E);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADC_A_H(void) {
    ADC_A_n(CPU_REG_H);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADC_A_L(void) {
    ADC_A_n(CPU_REG_L);

    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_ADC_A_HL(void) {
    ADC_A_n(mmu_get_byte(CPU_DREG_HL));

    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_ADC_A_d8(void) {
    uint8_t immediate = mmu_get_byte(cpu.PC + 1);
    ADC_A_n(immediate);

    cpu.cycle_count += 8;
    cpu.PC += 2;
}

static void SUB_A_n(uint8_t n) {
    uint8_t A      = CPU_REG_A;
    uint8_t result = A - n;

    clear_flag_register();
    set_flag(!result, Z_FLAG);
    set_flag(1, N_FLAG);
    set_flag((n & LO_NIBBLE_MASK) > (A & LO_NIBBLE_MASK), H_FLAG);
    set_flag(n > A, C_FLAG);

    CPU_REG_A = result;
}

void OPC_SUB_A_A(void) {
    SUB_A_n(CPU_REG_A);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SUB_A_B(void) {
    SUB_A_n(CPU_REG_B);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SUB_A_C(void) {
    SUB_A_n(CPU_REG_C);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SUB_A_D(void) {
    SUB_A_n(CPU_REG_D);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SUB_A_E(void) {
    SUB_A_n(CPU_REG_E);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SUB_A_H(void) {
    SUB_A_n(CPU_REG_H);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SUB_A_L(void) {
    SUB_A_n(CPU_REG_L);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SUB_A_HL(void) {
    SUB_A_n(mmu_get_byte(CPU_DREG_HL));
    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_SUB_A_d8(void) {
    uint8_t immediate = mmu_get_byte(cpu.PC + 1);
    SUB_A_n(immediate);
    cpu.cycle_count += 8;
    cpu.PC += 2;
}

static void SBC_A_n(uint8_t n) {
    uint8_t A      = CPU_REG_A;
    uint8_t carry  = get_flag(C_FLAG);
    uint8_t result = (uint8_t) (A - carry - n);

    clear_flag_register();
    set_flag(!result, Z_FLAG);
    set_flag(1, N_FLAG);
    set_flag(((n + carry) & LO_NIBBLE_MASK) > (A & LO_NIBBLE_MASK), H_FLAG);
    set_flag((n + carry) > A, C_FLAG);

    CPU_REG_A = result;
}

void OPC_SBC_A_A(void) {
    SBC_A_n(CPU_REG_A);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SBC_A_B(void) {
    SBC_A_n(CPU_REG_B);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SBC_A_C(void) {
    SBC_A_n(CPU_REG_C);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SBC_A_D(void) {
    SBC_A_n(CPU_REG_D);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SBC_A_E(void) {
    SBC_A_n(CPU_REG_E);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SBC_A_H(void) {
    SBC_A_n(CPU_REG_H);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SBC_A_L(void) {
    SBC_A_n(CPU_REG_L);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_SBC_A_HL(void) {
    SBC_A_n(mmu_get_byte(CPU_DREG_HL));
    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_SBC_A_d8(void) {
    uint8_t immediate = mmu_get_byte(cpu.PC + 1);
    SBC_A_n(immediate);
    cpu.cycle_count += 8;
    cpu.PC += 2;
}

static void AND_A_n(uint8_t n) {
    uint8_t result = CPU_REG_A & n;

    clear_flag_register(); // C and N are implicitly cleared
    set_flag(!result, Z_FLAG);
    set_flag(1, H_FLAG);

    CPU_REG_A = result;
}

void OPC_AND_A_A(void) {
    AND_A_n(CPU_REG_A);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_AND_A_B(void) {
    AND_A_n(CPU_REG_B);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_AND_A_C(void) {
    AND_A_n(CPU_REG_C);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_AND_A_D(void) {
    AND_A_n(CPU_REG_D);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_AND_A_E(void) {
    AND_A_n(CPU_REG_E);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_AND_A_H(void) {
    AND_A_n(CPU_REG_H);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_AND_A_L(void) {
    AND_A_n(CPU_REG_L);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_AND_A_HL(void) {
    AND_A_n(mmu_get_byte(CPU_DREG_HL));
    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_AND_A_d8(void) {
    uint8_t immediate = mmu_get_byte(cpu.PC + 1);
    AND_A_n(immediate);
    cpu.cycle_count += 8;
    cpu.PC += 2;
}

static void OR_A_n(uint8_t n) {
    uint8_t result = CPU_REG_A | n;

    clear_flag_register(); // C, H, and N are implicitly cleared
    set_flag(!result, Z_FLAG);

    CPU_REG_A = result;
}

void OPC_OR_A_A(void) {
    OR_A_n(CPU_REG_A);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_OR_A_B(void) {
    OR_A_n(CPU_REG_B);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_OR_A_C(void) {
    OR_A_n(CPU_REG_C);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_OR_A_D(void) {
    OR_A_n(CPU_REG_D);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_OR_A_E(void) {
    OR_A_n(CPU_REG_E);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_OR_A_H(void) {
    OR_A_n(CPU_REG_H);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_OR_A_L(void) {
    OR_A_n(CPU_REG_L);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_OR_A_HL(void) {
    OR_A_n(mmu_get_byte(CPU_DREG_HL));
    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_OR_A_d8(void) {
    uint8_t immediate = mmu_get_byte(cpu.PC + 1);
    OR_A_n(immediate);
    cpu.cycle_count += 8;
    cpu.PC += 2;
}

static void XOR_A_n(uint8_t n) {
    clear_flag_register();

    CPU_REG_A ^= n;
    set_flag((CPU_REG_A == 0x00), Z_FLAG);
}

void OPC_XOR_A_A(void) {
    XOR_A_n(CPU_REG_A);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_XOR_A_B(void) {
    XOR_A_n(CPU_REG_B);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_XOR_A_C(void) {
    XOR_A_n(CPU_REG_C);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_XOR_A_D(void) {
    XOR_A_n(CPU_REG_D);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_XOR_A_E(void) {
    XOR_A_n(CPU_REG_E);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_XOR_A_H(void) {
    XOR_A_n(CPU_REG_H);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_XOR_A_L(void) {
    XOR_A_n(CPU_REG_L);
    cpu.cycle_count += 4;
    ++cpu.PC;
}

void OPC_XOR_A_HL(void) {
    XOR_A_n(mmu_get_byte(CPU_DREG_HL));
    cpu.cycle_count += 8;
    ++cpu.PC;
}

void OPC_XOR_A_d8(void) {
    XOR_A_n(mmu_get_byte(cpu.PC + 1));
    cpu.cycle_count += 8;
    cpu.PC += 2;
}
