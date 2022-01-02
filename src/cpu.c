#include <stdio.h>
#include "cpu.h"
#include "mmu.h"
#include "rom.h"

struct CPU {
	uint8_t H;
	uint8_t L;

	uint8_t D;
	uint8_t E;

	uint8_t B;
	uint8_t C;

	uint8_t A;
	uint8_t F;

	uint16_t SP;
	uint16_t PC;
	uint16_t cycle_count;
};

struct CPU c;

void set_AF(uint16_t x) {
	c.F = x & 0xFF;
	c.A = x >> 8;
}

uint16_t get_AF() {
	return (c.A << 8) | c.F;
}

void set_BC(uint16_t x) {
	c.C = x & 0xFF;
	c.B = x >> 8;
}

uint16_t get_BC() {
	return (c.B << 8) | c.C;
}

void set_DE(uint16_t x) {
	c.E = x & 0xFF;
	c.D = x >> 8;
}

uint16_t get_DE() {
	return (c.D << 8) | c.E;
}

void set_HL(uint16_t x) {
	c.L = x & 0xFF;
	c.H = x >> 8;
}

uint16_t get_HL() {
	return (c.H << 8) | c.L;
}

/* ------------------ Flags */

uint8_t get_Z() { 
	return (c.F & 0x80); // 0x80 sets 8th bit to 1
}

void set_Z(uint8_t bit) { 
	c.F = (c.F & 0x7F) | (bit << 7); // 0x7F sets 8th bit to 0
}

uint8_t get_N() { 
	return (c.F & 0x40); // 0x40 sets 7th bit to 1
}

void set_N(uint8_t bit) { 
	c.F = (c.F & 0xBF) | (bit << 6); // 0xBF sets 7th bit to 0
}

uint8_t get_H() { 
	return (c.F & 0x20); // 0x20 sets 6th bit to 1
}

void set_H(uint8_t bit) { 
	c.F = (c.F & 0xDF) | (bit << 5); // 0xDF sets 6th bit to 0
}

uint8_t get_C() { 
	return (c.F & 0x10); // 0x10 sets 5th bit to 1
}

void set_C(uint8_t bit) { 
	c.F = (c.F & 0xEF) | (bit << 4); // 0xEF sets 5th bit to 0
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

uint16_t cpu_get_cycle_count() {
	return c.cycle_count;
}

uint16_t cpu_get_PC() {
	return c.PC;
}

void cpu_print_registers() {
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


uint8_t cpu_step() {
	uint8_t instruction = mmu_get_byte(c.PC);

 	printf("%04X: (%02X %02X %02X) A: %02X B: %02X C: %02X D: %02X E: %02X\n", 
            c.PC, instruction,
            mmu_get_byte(c.PC + 1), mmu_get_byte(c.PC + 2), c.A, c.B, c.C, c.D, c.E);

	c.PC++;
	// 16 bit helper variable
	uint16_t i = 0;

	switch(instruction) {
		case 0x00:			/* NOP */
			c.cycle_count++;
			break;
		case 0x01:		/* LD BC, 16-bit-val */
			i = mmu_get_two_bytes(c.PC);
			set_BC(i);
			c.PC += 2;
			c.cycle_count += 3;
			break;
		case 0x02:		/* LD (BC), A */
			mmu_write_byte(get_BC(), c.A);
			c.cycle_count++;
			break;
		case 0x03:		/* INC BC */
			set_BC(get_BC()+1);
			c.cycle_count += 2;
			break;
		case 0x04:		/* INC B */
			REG_INC(&c.B);
			break;
		case 0x05:		/* DEC B */
			REG_DEC(&c.B);
			break;
		case 0x06:		/* LD B, 8 bit value */
			LD_8(&c.B);
			break;
		case 0x0d:		/* DEC C */
			REG_DEC(&c.C);
			break;
		case 0x0e:		/* LD C, 8 bit value */
			LD_8(&c.C);
			break;
		case 0x12:		/* LD (DE), A */
			mmu_write_byte(get_DE(), c.A);
			c.cycle_count++;
			break;
		case 0x15:		/* DEC D */
			REG_DEC(&c.D);
			break;
		case 0x1D:		/* DEC E */
			REG_DEC(&c.E);
			break;
		case 0x20:
			if(!!((c.F & 0x80)) == 0) {
				c.PC += (uint8_t) mmu_get_byte(c.PC) + 1;
				c.cycle_count += 3;
			} else {
				c.PC += 1;
				c.cycle_count += 2;
			}
			break;
		case 0x21:
			i = mmu_get_two_bytes(c.PC);
			set_HL(i);
			c.PC += 2;
			c.cycle_count += 3;
			break;
		case 0x22:		/* LD (HL+), A */
			i = get_HL();
			mmu_write_byte(i, c.A);
			set_HL(i+1);
			c.cycle_count++;
			break;
		case 0x25:		/* DEC H */
			REG_DEC(&c.H);
			break;
		case 0x2D:		/* DEC L */
			REG_DEC(&c.L);
			break;
		case 0x31:		/* LD SP, 16-bit-value */
			c.SP = mmu_get_byte(c.PC);
			c.PC += 2;
			c.cycle_count += 3;
			break;
		case 0x32:		/* LD (HL-), A */
			c.cycle_count++;
			i = get_HL();
			mmu_write_byte(i, c.A);
			set_HL(i-1);
			c.cycle_count++;
			break;
		case 0x40:			/* LD B,B */
			LD_REG_REG(&c.B, c.B);
			break;
		case 0x41:			/* LD B,C */
			LD_REG_REG(&c.B, c.C);
			break;
		case 0x42:			/* LD B,D */
			LD_REG_REG(&c.B, c.D);
			break;
		case 0x43:			/* LD B,E */
			LD_REG_REG(&c.B, c.E);
			break;
		case 0x44:			/* LD B,H */
			LD_REG_REG(&c.B, c.H);
			break;
		case 0x45:			/* LD B,L */
			LD_REG_REG(&c.B, c.L);
			break;
		case 0x46:			/* LD D,HL */
			LD_REG_REG(&c.B, mmu_get_byte(get_HL()));
			c.cycle_count++;
			break;
		case 0x47:			/* LD B,A */
			LD_REG_REG(&c.B, c.A);
			break;
		case 0x48:			/* LD C,B */
			LD_REG_REG(&c.C, c.B);
			break;
		case 0x49:			/* LD C,C */
			LD_REG_REG(&c.C, c.C);
			break;
		case 0x4A:			/* LD C,D */
			LD_REG_REG(&c.C, c.D);
			break;
		case 0x4B:			/* LD C,E */
			LD_REG_REG(&c.C, c.E);
			break;
		case 0x4C:			/* LD C,H */
			LD_REG_REG(&c.C, c.H);
			break;
		case 0x4D:			/* LD C,L */
			LD_REG_REG(&c.C, c.L);
			break;
		case 0x4E:			/* LD C,HL */
			LD_REG_REG(&c.C, mmu_get_byte(get_HL()));
			c.cycle_count++;
			break;
		case 0x4F:			/* LD C,HL */
			LD_REG_REG(&c.C, c.A);
			break;
		case 0x50:			/* LD D,B */
			LD_REG_REG(&c.D, c.B);
			break;
		case 0x51:			/* LD D,C */
			LD_REG_REG(&c.D, c.C);
			break;
		case 0x52:			/* LD D,D */
			LD_REG_REG(&c.D, c.D);
			break;
		case 0x53:			/* LD D,E */
			LD_REG_REG(&c.D, c.E);
			break;
		case 0x54:			/* LD D,H */
			LD_REG_REG(&c.D, c.H);
			break;
		case 0x55:			/* LD D,L */
			LD_REG_REG(&c.D, c.L);
			break;
		case 0x56:			/* LD D,HL */
			LD_REG_REG(&c.D, mmu_get_byte(get_HL()));
			c.cycle_count++;
			break;
		case 0x57:			/* LD D,A */
			LD_REG_REG(&c.D, c.A);
			break;
		case 0x58:			/* LD E,B */
			LD_REG_REG(&c.E, c.B);
			break;
		case 0x59:			/* LD E,C */
			LD_REG_REG(&c.E, c.C);
			break;
		case 0x5A:			/* LD E,D */
			LD_REG_REG(&c.E, c.D);
			break;
		case 0x5B:			/* LD E,E */
			LD_REG_REG(&c.E, c.E);
			break;
		case 0x5C:			/* LD E,H */
			LD_REG_REG(&c.E, c.H);
			break;
		case 0x5D:			/* LD E,L */
			LD_REG_REG(&c.E, c.L);
			break;
		case 0x5E:			/* LD E,HL */
			LD_REG_REG(&c.E, mmu_get_byte(get_HL()));
			c.cycle_count++;
			break;
		case 0x5F:			/* LD E,A */
			LD_REG_REG(&c.E, c.A);
			c.cycle_count++;
			break;
		case 0x60:			/* LD H,B */
			LD_REG_REG(&c.H, c.B);
			break;
		case 0x61:			/* LD H,C */
			LD_REG_REG(&c.H, c.C);
			break;
		case 0x62:			/* LD H,D */
			LD_REG_REG(&c.H, c.D);
			break;
		case 0x63:			/* LD H,E */
			LD_REG_REG(&c.H, c.E);
			break;
		case 0x64:			/* LD H,H */
			LD_REG_REG(&c.H, c.H);
			break;
		case 0x65:			/* LD H,L */
			LD_REG_REG(&c.H, c.L);
			break;
		case 0x66:			/* LD H,HL */
			LD_REG_REG(&c.H, mmu_get_byte(get_HL()));
			c.cycle_count++;
			break;
		case 0x67:			/* LD H,A */
			LD_REG_REG(&c.H, c.A);
			break;
		case 0x68:			/* LD L,B */
			LD_REG_REG(&c.L, c.B);
			break;
		case 0x69:			/* LD L,C */
			LD_REG_REG(&c.L, c.C);
			break;
		case 0x6A:			/* LD L,D */
			LD_REG_REG(&c.L, c.D);
			break;
		case 0x6B:			/* LD L,E */
			LD_REG_REG(&c.L, c.E);
			break;
		case 0x6C:			/* LD L,H */
			LD_REG_REG(&c.L, c.H);
			break;
		case 0x6D:			/* LD L,L */
			LD_REG_REG(&c.L, c.L);
			break;
		case 0x6E:			/* LD L,HL */
			LD_REG_REG(&c.L, mmu_get_byte(get_HL()));
			c.cycle_count++;
			break;
		case 0x6F:			/* LD L,A */
			LD_REG_REG(&c.L, c.A);
			break;
		case 0x70:			/* LD HL,B */
			// TODO
			break;
		case 0x71:			/* LD HL,C */
			// TODO
			break;
		case 0x72:			/* LD HL,D */
			// TODO
			break;
		case 0x73:			/* LD HL,E */
			// TODO
			break;
		case 0x74:			/* LD HL,H */
			// TODO
			break;
		case 0x75:			/* LD HL,L */
			// TODO
			break;
		case 0x76:			/* HALT */
			// TODO
			break;
		case 0x77:			/* LD HL,A */
			// TODO
			break;
		case 0x78:			/* LD A,B */
			LD_REG_REG(&c.A, c.B);
			break;
		case 0x79:			/* LD A,C */
			LD_REG_REG(&c.A, c.C);
			break;
		case 0x7A:			/* LD A,D */
			LD_REG_REG(&c.A, c.D);
			break;
		case 0x7B:			/* LD A,E */
			LD_REG_REG(&c.A, c.E);
			break;
		case 0x7C:			/* LD A,H */
			LD_REG_REG(&c.A, c.H);
			break;
		case 0x7D:			/* LD A,L */
			LD_REG_REG(&c.A, c.L);
			break;
		case 0x7E:			/* LD A,HL */
			LD_REG_REG(&c.A, mmu_get_byte(get_HL()));
			c.cycle_count++;
			break;
		case 0x7F:			/* LD A,A */
			LD_REG_REG(&c.A, c.A);
			c.cycle_count++;
			break;
		case 0xA8:
			REG_XOR(&c.A, c.B);
			break;
		case 0xA9:
			REG_XOR(&c.A, c.C);
			break;
		case 0xAA:
			REG_XOR(&c.A, c.D);
			break;
		case 0xAB:
			REG_XOR(&c.A, c.E);
			break;
		case 0xAC:
			REG_XOR(&c.A, c.H);
			break;
		case 0xAD:
			REG_XOR(&c.A, c.L);
			break;
		case 0xAE:
			c.A ^= mmu_get_byte(get_HL());
			c.F = (!c.A) << 7;
			c.cycle_count++;
			break;
		case 0xAF:
			REG_XOR(&c.A, c.A);
			break;
		case 0xC3:
			c.PC = cpu_get_two_bytes(c.PC);
			c.cycle_count++;
			break;
        default:
			printf("Unsupported opcode: 0x%02x at 0x%04x\n", instruction, c.PC);
			return 1;
			break;
	}

	return 0;
}
