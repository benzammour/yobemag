#include <stdio.h>
#include "mmu.h"
#include "rom.h"
#include "cpu.h"

typedef void (*op_function)(void);
// Function is used for instruction array initialization, not recognized by compiler
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static void UNKNOWN_OPCODE(void) {
    fprintf(stderr, "Unallowed/Unimplemented OP Code: 0x%x\n", c.opcode);
}
#pragma GCC diagnostic pop

static op_function instr_lookup[0xFF + 1] = { [0 ... 0xFF] = UNKNOWN_OPCODE };
static op_function cb_prefixed_lookup[0xFF + 1] = { [0 ... 0xFF] = UNKNOWN_OPCODE };

cpu c = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

void set_AF(uint16_t x) {
    c.F = (uint8_t) x; // get lower octet
    c.A = (uint8_t) (x >> 8); // get higher octet
}

uint16_t get_AF(void) {
    return (uint16_t) ((c.A << 8) | c.F);
}

void set_BC(uint16_t x) {
    c.C = (uint8_t) x;
    c.B = (uint8_t) (x >> 8);
}

uint16_t get_BC(void) {
    return (uint16_t) ((c.B << 8) | c.C);
}

void set_DE(uint16_t x) {
    c.E = (uint8_t) x;
    c.D = (uint8_t) (x >> 8);
}

uint16_t get_DE(void) {
    return (uint16_t) ((c.D << 8) | c.E);
}

void set_HL(uint16_t x) {
    c.L = (uint8_t) x;
    c.H = (uint8_t) (x >> 8);
}

uint16_t get_HL(void) {
    return (uint16_t) ((c.H << 8) | c.L);
}

/* ------------------ Flags */

uint8_t get_Z(void) { 
    return (c.F & 0x80); // 0x80 sets 8th bit to 1
}

void set_Z(uint8_t bit) { 
    c.F = (uint8_t) ((c.F & 0x7F) | (bit << 7)); // 0x7F sets 8th bit to 0
}

uint8_t get_N(void) { 
    return (c.F & 0x40); // 0x40 sets 7th bit to 1
}

void set_N(uint8_t bit) { 
    c.F = (uint8_t) ((c.F & 0xBF) | (bit << 6)); // 0xBF sets 7th bit to 0
}

uint8_t get_H(void) { 
    return (c.F & 0x20); // 0x20 sets 6th bit to 1
}

void set_H(uint8_t bit) { 
    c.F = (uint8_t) ((c.F & 0xDF) | (bit << 5)); // 0xDF sets 6th bit to 0
}

uint8_t get_C(void) { 
    return (c.F & 0x10); // 0x10 sets 5th bit to 1
}

void set_C(uint8_t bit) { 
    c.F = (uint8_t) ((c.F & 0xEF) | (bit << 4)); // 0xEF sets 5th bit to 0
}

void LD_REG_REG(uint8_t *register_one, uint8_t register_two) {
    *register_one = register_two;
    c.cycle_count++;
}

void LD_8(uint8_t *addr) {
    *addr = mmu_get_byte(c.PC); 
    c.PC++; 
    c.cycle_count += 2;
}

void REG_XOR(uint8_t *register_x, uint8_t register_y) {
    *register_x ^= register_y; 
    set_Z(!register_x);
    set_H(0);
    set_N(0);
    set_C(0);
    c.cycle_count += 1;
}

void REG_DEC(uint8_t *reg) {
    (*reg)--;
    set_Z(!reg);
    set_N(1);
    set_H((*reg & 0xF) == 0xF);
    c.cycle_count += 1;
}

void REG_INC(uint8_t *reg) {
    (*reg)++;
    set_Z(!(*reg));
    set_H(((*reg) & 0xF) == 0);
    set_N(0);
    c.cycle_count += 1;
}

void optable_init(void) {
    // Set up lookup table
    instr_lookup[0x0] = OPC_NOP;
    instr_lookup[0x1] = OPC_LD_BC;
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
    set_AF(0x01B0);
    set_BC(0x0013);
    set_DE(0x00D8);
    set_HL(0x014D);
    c.SP = 0xFFFE;
    c.PC = 0x0;
    c.cycle_count = 0;
}

uint16_t cpu_get_cycle_count(void) {
    return c.cycle_count;
}

uint16_t cpu_get_PC(void) {
    return c.PC;
}

void cpu_print_registers(void) {
    printf("PC: %04X AF: %02X%02X, BC: %02X%02X, DE: %02X%02X, HL: %02X%02X, SP: %04X, cycles: %d\n",
                    c.PC, c.A, c.F, c.B, c.C, c.D, c.E, c.H, c.L, c.SP, c.cycle_count);
}

uint16_t cpu_get_two_bytes(uint16_t addr) {
    uint16_t value = mmu_get_byte(addr);
    c.cycle_count++;
    value |= (mmu_get_byte(addr+1)<<8);
    c.cycle_count++;

    return value;
}

uint8_t cpu_step(void) {
    c.opcode = mmu_get_byte(c.PC);


     printf("%04X: (%02X %02X %02X) A: %02X B: %02X C: %02X D: %02X E: %02X\n", 
            c.PC, c.opcode,
            mmu_get_byte(c.PC + 1), mmu_get_byte(c.PC + 2), c.A, c.B, c.C, c.D, c.E);

    c.PC++;
    
    // Get and Execute c.opcode 
    (*(instr_lookup[c.opcode]))();

    return 0;
}

// OP-Codes
void OPC_NOP(void) {
    c.cycle_count++;
}

void OPC_LD_BC(void) {
    uint16_t i = mmu_get_two_bytes(c.PC);
    set_BC(i);

    c.PC += 2;
    c.cycle_count += 3;
}

void OPC_LD_BC_A(void) {
    mmu_write_byte(get_BC(), c.A);
    c.cycle_count++;
}

void OPC_INC_BC(void) {
    set_BC(get_BC()+1);
    c.cycle_count += 2;
}

void OPC_INC_B(void) {
    REG_INC(&c.B);
}

void OPC_DEC_B(void) {
    REG_DEC(&c.B);
}

void OPC_LD_B(void) {
    LD_8(&c.B);
}

void OPC_DEC_C(void) {
    REG_DEC(&c.C);
}

void OPC_LD_C(void) {
    LD_8(&c.C);
}

void OPC_LD_DE_A(void) {
    mmu_write_byte(get_DE(), c.A);
    c.cycle_count++;
}

void OPC_DEC_D(void) {
    REG_DEC(&c.D);
}

void OPC_DEC_E(void) { 
    REG_DEC(&c.E);
}
    
void OPC_LD_HL_PLUS_A(void) {
    uint16_t i = get_HL();
    mmu_write_byte(i, c.A);
    set_HL(i+1);
    c.cycle_count++;
}

void OPC_DEC_H(void) {
    REG_DEC(&c.H);
}

void OPC_DEC_L(void) {
    REG_DEC(&c.L);
}

void OPC_LD_SP(void) {
    c.SP = mmu_get_byte(c.PC);
    c.PC += 2;
    c.cycle_count += 3;
}

void OPC_LD_HL_MINUS_A(void) { 
    c.cycle_count++;
    uint16_t i = get_HL();
    mmu_write_byte(i, c.A);
    set_HL(i-1);
    c.cycle_count++;
}

void OPC_LD_B_B(void) {
    LD_REG_REG(&c.B, c.B);
}

void OPC_LD_B_C(void) { 
    LD_REG_REG(&c.B, c.C);
}

void OPC_LD_B_D(void) { 
    LD_REG_REG(&c.B, c.D);
}

void OPC_LD_B_E(void) {
    LD_REG_REG(&c.B, c.E);
}

void OPC_LD_B_H(void) {
    LD_REG_REG(&c.B, c.H);
}

void OPC_LD_B_L(void) { 
    LD_REG_REG(&c.B, c.L);
}

void OPC_LD_B_HL(void) { 
    LD_REG_REG(&c.B, mmu_get_byte(get_HL()));
    c.cycle_count++;
}

void OPC_LD_B_A(void) {
    LD_REG_REG(&c.B, c.A);
}

void OPC_LD_C_B(void) { 
    LD_REG_REG(&c.C, c.B);
}

void OPC_LD_C_C(void) {
    LD_REG_REG(&c.C, c.C);
}

void OPC_LD_C_D(void) { 
    LD_REG_REG(&c.C, c.D);
}

void OPC_LD_C_E(void) {
    LD_REG_REG(&c.C, c.E);
}

void OPC_LD_C_H(void) { 
    LD_REG_REG(&c.C, c.H);
}

void OPC_LD_C_L(void) { 
    LD_REG_REG(&c.C, c.L);
}

void OPC_LD_C_HL(void) { 
    LD_REG_REG(&c.C, mmu_get_byte(get_HL()));
    c.cycle_count++;
}

void OPC_LD_C_A(void) { 
    LD_REG_REG(&c.C, c.A);
}

void OPC_LD_D_B(void) { 
    LD_REG_REG(&c.D, c.B);
}

void OPC_LD_D_C(void) { 
    LD_REG_REG(&c.D, c.C);
}

void OPC_LD_D_D(void) { 
    LD_REG_REG(&c.D, c.D);
}

void OPC_LD_D_E(void) { 
    LD_REG_REG(&c.D, c.E);
}

void OPC_LD_D_H(void) { 
    LD_REG_REG(&c.D, c.H);
}

void OPC_LD_D_L(void) { 
    LD_REG_REG(&c.D, c.L);
}

void OPC_LD_D_HL(void) { 
    LD_REG_REG(&c.D, mmu_get_byte(get_HL()));
    c.cycle_count++;
}

void OPC_LD_D_A(void) { 
    LD_REG_REG(&c.D, c.A);
}

void OPC_LD_E_B(void) { 
    LD_REG_REG(&c.E, c.B);
}

void OPC_LD_E_C(void) { 
    LD_REG_REG(&c.E, c.C);
}

void OPC_LD_E_D(void) { 
    LD_REG_REG(&c.E, c.D);
}

void OPC_LD_E_E(void) { 
    LD_REG_REG(&c.E, c.E);
}

void OPC_LD_E_H(void) { 
    LD_REG_REG(&c.E, c.H);
}

void OPC_LD_E_L(void) { 
    LD_REG_REG(&c.E, c.L);
}

void OPC_LD_E_HL(void) { 
    LD_REG_REG(&c.E, mmu_get_byte(get_HL()));
    c.cycle_count++;
}

void OPC_LD_E_A(void) { 
    LD_REG_REG(&c.E, c.A);
    c.cycle_count++;
}

void OPC_LD_H_B(void) { 
    LD_REG_REG(&c.H, c.B);
}

void OPC_LD_H_C(void) { 
    LD_REG_REG(&c.H, c.C);
}

void OPC_LD_H_D(void) { 
    LD_REG_REG(&c.H, c.D);
}

void OPC_LD_H_E(void) { 
    LD_REG_REG(&c.H, c.E);
}

void OPC_LD_H_H(void) { 
    LD_REG_REG(&c.H, c.H);
}

void OPC_LD_H_L(void) { 
    LD_REG_REG(&c.H, c.L);
}

void OPC_LD_H_HL(void) { 
    LD_REG_REG(&c.H, mmu_get_byte(get_HL()));
    c.cycle_count++;
}

void OPC_LD_H_A(void) { 
    LD_REG_REG(&c.H, c.A);
}

void OPC_LD_L_B(void) { 
    LD_REG_REG(&c.L, c.B);
}

void OPC_LD_L_C(void) { 
    LD_REG_REG(&c.L, c.C);
}

void OPC_LD_L_D(void) { 
    LD_REG_REG(&c.L, c.D);
}

void OPC_LD_L_E(void) { 
    LD_REG_REG(&c.L, c.E);
}

void OPC_LD_L_H(void) { 
    LD_REG_REG(&c.L, c.H);
}

void OPC_LD_L_L(void) { 
    LD_REG_REG(&c.L, c.L);
}

void OPC_LD_L_HL(void) { 
    LD_REG_REG(&c.L, mmu_get_byte(get_HL()));
    c.cycle_count++;
}

void OPC_LD_L_A(void) { 
    LD_REG_REG(&c.L, c.A);
}

void OPC_LD_A_B(void) { 
    LD_REG_REG(&c.A, c.B);
}

void OPC_LD_A_C(void) { 
    LD_REG_REG(&c.A, c.C);
}

void OPC_LD_A_D(void) { 
    LD_REG_REG(&c.A, c.D);
}

void OPC_LD_A_E(void) { 
    LD_REG_REG(&c.A, c.E);
}

void OPC_LD_A_H(void) { 
    LD_REG_REG(&c.A, c.H);
}

void OPC_LD_A_L(void) { 
    LD_REG_REG(&c.A, c.L);
}

void OPC_LD_A_HL(void) { 
    LD_REG_REG(&c.A, mmu_get_byte(get_HL()));
    c.cycle_count++;
}

void OPC_LD_A_A(void) { 
    LD_REG_REG(&c.A, c.A);
    c.cycle_count++;
}
