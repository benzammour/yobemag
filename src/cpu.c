#include <stdio.h>

#include "mmu.h"
#include "rom.h"
#include "cpu.h"
#include "logging.h"

#define LO_NIBBLE_MASK (0x0F)
//#define HI_NIBBLE_MASK (0xF0)
#define BYTE_MASK (0xFF)

typedef void (*op_function)(void);

// Function is used for instruction array initialization, not recognized by compiler
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static void UNKNOWN_OPCODE(void) {
    LOG_ERROR("Unallowed/Unimplemented OP Code: 0x%x", cpu.opcode);
}
#pragma GCC diagnostic pop

static op_function instr_lookup[0xFF + 1] = { [0 ... 0xFF] = UNKNOWN_OPCODE };
static op_function cb_prefixed_lookup[0xFF + 1] = { [0 ... 0xFF] = UNKNOWN_OPCODE };

CPU cpu = {
    .HL.word = 0,
    .DE.word = 0,
    .BC.word = 0,
    .AF.word = 0,
	0,
	0,
    0,
    0,
};

__attribute((always_inline)) inline static void set_flag(uint8_t bit, Flag f) {
	cpu.AF.bytes.low |= bit << f;
}

__attribute((always_inline)) inline static void clear_flag(uint8_t bit, Flag f) {
	cpu.AF.bytes.low &= (uint8_t) ~(bit << f);
}

__attribute((always_inline)) inline static void clear_flag_register(void) {
	cpu.AF.bytes.low = 0;
}

void LD_REG_REG(uint8_t *register_one, uint8_t register_two) {
    *register_one = register_two;
    cpu.cycle_count++;
}

void LD_8(uint8_t *addr) {
    *addr = mmu_get_byte(cpu.PC);
    cpu.PC++;
	cpu.cycle_count += 2;
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
	clear_flag(1, N_FLAG);
	set_flag(!(*reg), Z_FLAG);

	// TODO: This is 4 in most and 12 in one case,
	//  	 add where this function is called
	cpu.cycle_count += 1;
}

void optable_init(void) {
    // Set up lookup table
    instr_lookup[0x00] = OPC_NOP;
    instr_lookup[0x01] = OPC_LD_BC;
    instr_lookup[0x02] = OPC_LD_BC_A;
    instr_lookup[0x03] = OPC_INC_BC;
    instr_lookup[0x04] = OPC_INC_B;
    instr_lookup[0x05] = OPC_DEC_B;
    instr_lookup[0x06] = OPC_LD_B;
    instr_lookup[0x0d] = OPC_DEC_C;
    instr_lookup[0x0e] = OPC_LD_C;
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
    instr_lookup[0x40] = OPC_LD_B_B;
    instr_lookup[0x41] = OPC_LD_B_C;
    instr_lookup[0x42] = OPC_LD_B_D;
    instr_lookup[0x43] = OPC_LD_B_E;
    instr_lookup[0x44] = OPC_LD_B_H;
    instr_lookup[0x45] = OPC_LD_B_L;
    instr_lookup[0x46] = OPC_LD_D_HL;
    instr_lookup[0x47] = OPC_LD_B_A;
    instr_lookup[0x48] = OPC_LD_C_B;
    instr_lookup[0x49] = OPC_LD_C_C;
    instr_lookup[0x4A] = OPC_LD_C_D;
    instr_lookup[0x4B] = OPC_LD_C_E;
    instr_lookup[0x4C] = OPC_LD_C_H;
    instr_lookup[0x4D] = OPC_LD_C_L;
    instr_lookup[0x4E] = OPC_LD_C_HL;
    instr_lookup[0x4F] = OPC_LD_C_HL;
    instr_lookup[0x50] = OPC_LD_D_B;
    instr_lookup[0x51] = OPC_LD_D_C;
    instr_lookup[0x52] = OPC_LD_D_D;
    instr_lookup[0x53] = OPC_LD_D_E;
    instr_lookup[0x54] = OPC_LD_D_H;
    instr_lookup[0x55] = OPC_LD_D_L;
    instr_lookup[0x56] = OPC_LD_D_HL;
    instr_lookup[0x57] = OPC_LD_D_A;
    instr_lookup[0x58] = OPC_LD_E_B;
    instr_lookup[0x59] = OPC_LD_E_C;
    instr_lookup[0x5A] = OPC_LD_E_D;
    instr_lookup[0x5B] = OPC_LD_E_E;
    instr_lookup[0x5C] = OPC_LD_E_H;
    instr_lookup[0x5D] = OPC_LD_E_L;
    instr_lookup[0x5E] = OPC_LD_E_HL;
    instr_lookup[0x5F] = OPC_LD_E_A;
    instr_lookup[0x60] = OPC_LD_H_B;
    instr_lookup[0x61] = OPC_LD_H_C;
    instr_lookup[0x62] = OPC_LD_H_D;
    instr_lookup[0x63] = OPC_LD_H_E;
    instr_lookup[0x64] = OPC_LD_H_H;
    instr_lookup[0x65] = OPC_LD_H_L;
    instr_lookup[0x66] = OPC_LD_H_HL;
    instr_lookup[0x67] = OPC_LD_H_A;
    instr_lookup[0x68] = OPC_LD_L_B;
    instr_lookup[0x69] = OPC_LD_L_C;
    instr_lookup[0x6A] = OPC_LD_L_D;
    instr_lookup[0x6B] = OPC_LD_L_E;
    instr_lookup[0x6C] = OPC_LD_L_H;
    instr_lookup[0x6D] = OPC_LD_L_L;
    instr_lookup[0x6E] = OPC_LD_L_HL;
    instr_lookup[0x6F] = OPC_LD_L_A;
    instr_lookup[0x78] = OPC_LD_A_B;
    instr_lookup[0x79] = OPC_LD_A_C;
    instr_lookup[0x7A] = OPC_LD_A_D;
    instr_lookup[0x7B] = OPC_LD_A_E;
    instr_lookup[0x7C] = OPC_LD_A_H;
    instr_lookup[0x7D] = OPC_LD_A_L;
    instr_lookup[0x7E] = OPC_LD_A_HL;
    instr_lookup[0x7F] = OPC_LD_A_A;

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

    // TODO: 0xA8
    // TODO: 0xA9
    // TODO: 0xAA
    // TODO: 0xAB
    // TODO: 0xAC
    // TODO: 0xAD
    // TODO: 0xAE
    // TODO: 0xAF
    // TODO: 0xC3

    cb_prefixed_lookup[0x0] = UNKNOWN_OPCODE;
}


/* ------------------ CPU Funcs */
void cpu_init(void) {
    optable_init();

	cpu.AF.word = 0x01B0;
    cpu.BC.word = 0x0013;
	cpu.DE.word = 0x00D8;
	cpu.HL.word = 0x014D;
	cpu.SP = 0xFFFE;
	cpu.PC = 0x0;
	cpu.cycle_count = 0;
}

uint16_t cpu_get_cycle_count(void) {
    return cpu.cycle_count;
}

uint16_t cpu_get_PC(void) {
    return cpu.PC;
}

void cpu_print_registers(void) {
    LOG_DEBUG("PC: %04X AF: %02X%02X, BC: %02X%02X, DE: %02X%02X, HL: %02X%02X, SP: %04X, cycles: %d",
		   cpu.PC, cpu.AF.bytes.high, cpu.AF.bytes.low, cpu.BC.bytes.high, cpu.BC.bytes.low, cpu.DE.bytes.high, cpu.DE.bytes.low, cpu.HL.bytes.high, cpu.HL.bytes.low, cpu.SP, cpu.cycle_count);
}

uint16_t cpu_get_two_bytes(uint16_t addr) {
    uint16_t value = mmu_get_byte(addr);
    cpu.cycle_count++;
    value |= (mmu_get_byte(addr+1)<<8);
    cpu.cycle_count++;

    return value;
}

ErrorCode cpu_step(void) {
	cpu.opcode = mmu_get_byte(cpu.PC);


     LOG_DEBUG("%04X: (%02X %02X %02X) A: %02X B: %02X C: %02X D: %02X E: %02X",
			cpu.PC, cpu.opcode,
			mmu_get_byte(cpu.PC + 1), mmu_get_byte(cpu.PC + 2), cpu.AF.bytes.high, cpu.BC.bytes.high, cpu.BC.bytes.low, cpu.DE.bytes.high, cpu.DE.bytes.low);

    // Get and Execute c.opcode
    (*(instr_lookup[cpu.opcode]))();

	cpu.PC++;

    return ERR_SUCCESS;
}

// OP-Codes
void OPC_NOP(void) {
    cpu.cycle_count++;
}

void OPC_LD_BC(void) {
    uint16_t i = mmu_get_two_bytes(cpu.PC);
    cpu.BC.word = i;

	cpu.PC += 2;
	cpu.cycle_count += 3;
}

void OPC_LD_BC_A(void) {
    mmu_write_byte(cpu.BC.word, cpu.AF.bytes.high);
    cpu.cycle_count++;
}

void OPC_INC_BC(void) {
    ++cpu.BC.word;
	cpu.cycle_count += 2;
}

void OPC_INC_B(void) {
    REG_INC(&cpu.BC.bytes.high);
}

void OPC_DEC_B(void) {
    REG_DEC(&cpu.BC.bytes.high);
}

void OPC_LD_B(void) {
    LD_8(&cpu.BC.bytes.high);
}

void OPC_DEC_C(void) {
    REG_DEC(&cpu.BC.bytes.low);
}

void OPC_LD_C(void) {
    LD_8(&cpu.BC.bytes.low);
}

void OPC_LD_DE_A(void) {
    mmu_write_byte(cpu.DE.word, cpu.AF.bytes.high);
    cpu.cycle_count++;
}

void OPC_DEC_D(void) {
    REG_DEC(&cpu.DE.bytes.high);
}

void OPC_DEC_E(void) {
    REG_DEC(&cpu.DE.bytes.low);
}

void OPC_LD_HL_PLUS_A(void) {
    uint16_t i = cpu.HL.word;
    mmu_write_byte(i, cpu.AF.bytes.high);
    cpu.HL.word = i+1;
    cpu.cycle_count++;
}

void OPC_DEC_H(void) {
    REG_DEC(&cpu.HL.bytes.high);
}

void OPC_DEC_L(void) {
    REG_DEC(&cpu.HL.bytes.low);
}

void OPC_LD_SP(void) {
	cpu.SP = mmu_get_byte(cpu.PC);
	cpu.PC += 2;
	cpu.cycle_count += 3;
}

void OPC_LD_HL_MINUS_A(void) {
    cpu.cycle_count++;
    uint16_t i = cpu.HL.word;
    mmu_write_byte(i, cpu.AF.bytes.high);
    cpu.HL.word = i-1;
    cpu.cycle_count++;
}

void OPC_LD_B_B(void) {
    LD_REG_REG(&cpu.BC.bytes.high, cpu.BC.bytes.high);
}

void OPC_LD_B_C(void) {
    LD_REG_REG(&cpu.BC.bytes.high, cpu.BC.bytes.low);
}

void OPC_LD_B_D(void) {
    LD_REG_REG(&cpu.BC.bytes.high, cpu.DE.bytes.high);
}

void OPC_LD_B_E(void) {
    LD_REG_REG(&cpu.BC.bytes.high, cpu.DE.bytes.low);
}

void OPC_LD_B_H(void) {
    LD_REG_REG(&cpu.BC.bytes.high, cpu.HL.bytes.high);
}

void OPC_LD_B_L(void) {
    LD_REG_REG(&cpu.BC.bytes.high, cpu.HL.bytes.low);
}

void OPC_LD_B_HL(void) {
    LD_REG_REG(&cpu.BC.bytes.high, mmu_get_byte(cpu.HL.word));
    cpu.cycle_count++;
}

void OPC_LD_B_A(void) {
    LD_REG_REG(&cpu.BC.bytes.high, cpu.AF.bytes.high);
}

void OPC_LD_C_B(void) {
    LD_REG_REG(&cpu.BC.bytes.low, cpu.BC.bytes.high);
}

void OPC_LD_C_C(void) {
    LD_REG_REG(&cpu.BC.bytes.low, cpu.BC.bytes.low);
}

void OPC_LD_C_D(void) {
    LD_REG_REG(&cpu.BC.bytes.low, cpu.DE.bytes.high);
}

void OPC_LD_C_E(void) {
    LD_REG_REG(&cpu.BC.bytes.low, cpu.DE.bytes.low);
}

void OPC_LD_C_H(void) {
    LD_REG_REG(&cpu.BC.bytes.low, cpu.HL.bytes.high);
}

void OPC_LD_C_L(void) {
    LD_REG_REG(&cpu.BC.bytes.low, cpu.HL.bytes.low);
}

void OPC_LD_C_HL(void) {
    LD_REG_REG(&cpu.BC.bytes.low, mmu_get_byte(cpu.HL.word));
    cpu.cycle_count++;
}

void OPC_LD_C_A(void) {
    LD_REG_REG(&cpu.BC.bytes.low, cpu.AF.bytes.high);
}

void OPC_LD_D_B(void) {
    LD_REG_REG(&cpu.DE.bytes.high, cpu.BC.bytes.high);
}

void OPC_LD_D_C(void) {
    LD_REG_REG(&cpu.DE.bytes.high, cpu.BC.bytes.low);
}

void OPC_LD_D_D(void) {
    LD_REG_REG(&cpu.DE.bytes.high, cpu.DE.bytes.high);
}

void OPC_LD_D_E(void) {
    LD_REG_REG(&cpu.DE.bytes.high, cpu.DE.bytes.low);
}

void OPC_LD_D_H(void) {
    LD_REG_REG(&cpu.DE.bytes.high, cpu.HL.bytes.high);
}

void OPC_LD_D_L(void) {
    LD_REG_REG(&cpu.DE.bytes.high, cpu.HL.bytes.low);
}

void OPC_LD_D_HL(void) {
    LD_REG_REG(&cpu.DE.bytes.high, mmu_get_byte(cpu.HL.word));
    cpu.cycle_count++;
}

void OPC_LD_D_A(void) {
    LD_REG_REG(&cpu.DE.bytes.high, cpu.AF.bytes.high);
}

void OPC_LD_E_B(void) {
    LD_REG_REG(&cpu.DE.bytes.low, cpu.BC.bytes.high);
}

void OPC_LD_E_C(void) {
    LD_REG_REG(&cpu.DE.bytes.low, cpu.BC.bytes.low);
}

void OPC_LD_E_D(void) {
    LD_REG_REG(&cpu.DE.bytes.low, cpu.DE.bytes.high);
}

void OPC_LD_E_E(void) {
    LD_REG_REG(&cpu.DE.bytes.low, cpu.DE.bytes.low);
}

void OPC_LD_E_H(void) {
    LD_REG_REG(&cpu.DE.bytes.low, cpu.HL.bytes.high);
}

void OPC_LD_E_L(void) {
    LD_REG_REG(&cpu.DE.bytes.low, cpu.HL.bytes.low);
}

void OPC_LD_E_HL(void) {
    LD_REG_REG(&cpu.DE.bytes.low, mmu_get_byte(cpu.HL.word));
    cpu.cycle_count++;
}

void OPC_LD_E_A(void) {
    LD_REG_REG(&cpu.DE.bytes.low, cpu.AF.bytes.high);
    cpu.cycle_count++;
}

void OPC_LD_H_B(void) {
    LD_REG_REG(&cpu.HL.bytes.high, cpu.BC.bytes.high);
}

void OPC_LD_H_C(void) {
    LD_REG_REG(&cpu.HL.bytes.high, cpu.BC.bytes.low);
}

void OPC_LD_H_D(void) {
    LD_REG_REG(&cpu.HL.bytes.high, cpu.DE.bytes.high);
}

void OPC_LD_H_E(void) {
    LD_REG_REG(&cpu.HL.bytes.high, cpu.DE.bytes.low);
}

void OPC_LD_H_H(void) {
    LD_REG_REG(&cpu.HL.bytes.high, cpu.HL.bytes.high);
}

void OPC_LD_H_L(void) {
    LD_REG_REG(&cpu.HL.bytes.high, cpu.HL.bytes.low);
}

void OPC_LD_H_HL(void) {
    LD_REG_REG(&cpu.HL.bytes.high, mmu_get_byte(cpu.HL.word));
    cpu.cycle_count++;
}

void OPC_LD_H_A(void) {
    LD_REG_REG(&cpu.HL.bytes.high, cpu.AF.bytes.high);
}

void OPC_LD_L_B(void) {
    LD_REG_REG(&cpu.HL.bytes.low, cpu.BC.bytes.high);
}

void OPC_LD_L_C(void) {
    LD_REG_REG(&cpu.HL.bytes.low, cpu.BC.bytes.low);
}

void OPC_LD_L_D(void) {
    LD_REG_REG(&cpu.HL.bytes.low, cpu.DE.bytes.high);
}

void OPC_LD_L_E(void) {
    LD_REG_REG(&cpu.HL.bytes.low, cpu.DE.bytes.low);
}

void OPC_LD_L_H(void) {
    LD_REG_REG(&cpu.HL.bytes.low, cpu.HL.bytes.high);
}

void OPC_LD_L_L(void) {
    LD_REG_REG(&cpu.HL.bytes.low, cpu.HL.bytes.low);
}

void OPC_LD_L_HL(void) {
    LD_REG_REG(&cpu.HL.bytes.low, mmu_get_byte(cpu.HL.word));
    cpu.cycle_count++;
}

void OPC_LD_L_A(void) {
    LD_REG_REG(&cpu.HL.bytes.low, cpu.AF.bytes.high);
}

void OPC_LD_A_B(void) {
    LD_REG_REG(&cpu.AF.bytes.high, cpu.BC.bytes.high);
}

void OPC_LD_A_C(void) {
    LD_REG_REG(&cpu.AF.bytes.high, cpu.BC.bytes.low);
}

void OPC_LD_A_D(void) {
    LD_REG_REG(&cpu.AF.bytes.high, cpu.DE.bytes.high);
}

void OPC_LD_A_E(void) {
    LD_REG_REG(&cpu.AF.bytes.high, cpu.DE.bytes.low);
}

void OPC_LD_A_H(void) {
    LD_REG_REG(&cpu.AF.bytes.high, cpu.HL.bytes.high);
}

void OPC_LD_A_L(void) {
    LD_REG_REG(&cpu.AF.bytes.high, cpu.HL.bytes.low);
}

void OPC_LD_A_HL(void) {
    LD_REG_REG(&cpu.AF.bytes.high, mmu_get_byte(cpu.HL.word));
    cpu.cycle_count++;
}

void OPC_LD_A_A(void) {
    LD_REG_REG(&cpu.AF.bytes.high, cpu.AF.bytes.high);
    cpu.cycle_count++;
}

/******************************************************
 *** 8-BIT ALU                                      ***
 ******************************************************/

static void ADD_A_n(uint8_t n) {
	uint8_t A = cpu.AF.bytes.high;
	uint8_t A_nibble = A & LO_NIBBLE_MASK;
	uint8_t n_nibble = n & LO_NIBBLE_MASK;
	uint_fast16_t result = A + n;

	clear_flag_register();
	set_flag(result > BYTE_MASK, C_FLAG);
	set_flag(A_nibble + n_nibble > LO_NIBBLE_MASK, H_FLAG);
	set_flag((result &= BYTE_MASK) == 0, Z_FLAG);

	cpu.AF.bytes.high = (uint8_t) result;
}

void OPC_ADD_A_A(void) {
	ADD_A_n(cpu.AF.bytes.high);
	cpu.cycle_count += 4;
}

void OPC_ADD_A_B(void) {
	ADD_A_n(cpu.BC.bytes.high);
	cpu.cycle_count += 4;
}

void OPC_ADD_A_C(void) {
	ADD_A_n(cpu.BC.bytes.low);
	cpu.cycle_count += 4;
}

void OPC_ADD_A_D(void) {
	ADD_A_n(cpu.DE.bytes.high);
	cpu.cycle_count += 4;
}

void OPC_ADD_A_E(void) {
	ADD_A_n(cpu.DE.bytes.low);
	cpu.cycle_count += 4;
}

void OPC_ADD_A_H(void) {
	ADD_A_n(cpu.HL.bytes.high);
	cpu.cycle_count += 4;
}

void OPC_ADD_A_L(void) {
	ADD_A_n(cpu.HL.bytes.low);
	cpu.cycle_count += 4;
}

void OPC_ADD_A_HL(void) {
	ADD_A_n(mmu_get_byte(cpu.HL.word));
	cpu.cycle_count += 8;
}

void OPC_ADD_A_d8(void) {
	uint8_t immediate = mmu_get_byte(cpu.PC + 1);
	ADD_A_n(immediate);
	cpu.cycle_count += 8;
}


static void SUB_A_n(uint8_t n) {
	uint8_t A = cpu.AF.bytes.high;
	uint_fast16_t result = A - n;

	clear_flag_register();
	set_flag(!result, Z_FLAG);
	set_flag(1, N_FLAG);
	set_flag((n & LO_NIBBLE_MASK) > (A & LO_NIBBLE_MASK), H_FLAG);
	set_flag(result > 0, C_FLAG);

	cpu.AF.bytes.high = (uint8_t) result;
}

void OPC_SUB_A_A(void) {
	SUB_A_n(cpu.AF.bytes.high);
	cpu.cycle_count += 4;
}

void OPC_SUB_A_B(void) {
	SUB_A_n(cpu.BC.bytes.high);
	cpu.cycle_count += 4;
}

void OPC_SUB_A_C(void) {
	SUB_A_n(cpu.BC.bytes.low);
	cpu.cycle_count += 4;
}

void OPC_SUB_A_D(void) {
	SUB_A_n(cpu.DE.bytes.high);
	cpu.cycle_count += 4;
}

void OPC_SUB_A_E(void) {
	SUB_A_n(cpu.DE.bytes.low);
	cpu.cycle_count += 4;
}

void OPC_SUB_A_H(void) {
	SUB_A_n(cpu.HL.bytes.high);
	cpu.cycle_count += 4;
}

void OPC_SUB_A_L(void) {
	SUB_A_n(cpu.HL.bytes.low);
	cpu.cycle_count += 4;
}

void OPC_SUB_A_HL(void) {
	SUB_A_n(mmu_get_byte(cpu.HL.word));
	cpu.cycle_count += 8;
}

void OPC_SUB_A_d8(void) {
	uint8_t immediate = mmu_get_byte(cpu.PC + 1);
	SUB_A_n(immediate);
	cpu.cycle_count += 8;
}
