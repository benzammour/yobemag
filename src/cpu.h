#ifndef YOBEMAG_CPU_H
#define YOBEMAG_CPU_H

#include <stdint.h>

#include "errcodes.h"

typedef enum flag {
	/**
	 * @brief Carry flag
	 */
	C_FLAG = 4,
	/**
	 * @brief Half carry flag
	 */
	H_FLAG = 5,
	/**
	 * @brief Subtract flag
	 */
	N_FLAG = 6,
	/**
	 * @brief Zero flag
	 */
	Z_FLAG = 7,
} Flag;

typedef union Register {
	struct {
		uint8_t high; // A, B, D, H
		uint8_t low; // F, C, E, L
	} bytes;
	uint16_t word;
} Register;

typedef struct CPU {
	Register HL;
	Register DE;
	Register BC;
	Register AF;

    uint16_t SP;
    uint16_t PC;

    uint16_t cycle_count;

    uint8_t opcode;
} CPU;

extern CPU cpu;

void cpu_init(void);
__attribute__((pure)) uint16_t cpu_get_cycle_count(void);
ErrorCode cpu_step(void);


__attribute__((pure)) uint16_t cpu_get_PC(void);
void cpu_print_registers(void);

void LD_REG_REG(uint8_t *register_one, uint8_t register_two);
void LD_8(uint8_t *addr);
void REG_XOR(uint8_t *register_x, uint8_t register_y);
void REG_DEC(uint8_t *reg);
void REG_INC(uint8_t *reg);

void optable_init(void);

void OPC_NOP(void);
void OPC_LD_BC(void);
void OPC_LD_BC_A(void);
void OPC_INC_BC(void);
void OPC_INC_B(void);
void OPC_DEC_B(void);
void OPC_LD_B(void);
void OPC_DEC_C(void);
void OPC_LD_C(void);
void OPC_LD_DE_A(void);
void OPC_DEC_D(void);
void OPC_DEC_E(void);
void OPC_LD_HL_PLUS_A(void);
void OPC_DEC_H(void);
void OPC_DEC_L(void);
void OPC_LD_SP(void);
void OPC_LD_HL_MINUS_A(void);
void OPC_LD_B_B(void);
void OPC_LD_B_C(void);
void OPC_LD_B_D(void);
void OPC_LD_B_E(void);
void OPC_LD_B_H(void);
void OPC_LD_B_L(void);
void OPC_LD_B_HL(void);
void OPC_LD_B_A(void);
void OPC_LD_C_B(void);
void OPC_LD_C_C(void);
void OPC_LD_C_D(void);
void OPC_LD_C_E(void);
void OPC_LD_C_H(void);
void OPC_LD_C_L(void);
void OPC_LD_C_HL(void);
void OPC_LD_C_A(void);
void OPC_LD_D_B(void);
void OPC_LD_D_C(void);
void OPC_LD_D_D(void);
void OPC_LD_D_E(void);
void OPC_LD_D_H(void);
void OPC_LD_D_L(void);
void OPC_LD_D_HL(void);
void OPC_LD_D_A(void);
void OPC_LD_E_B(void);
void OPC_LD_E_C(void);
void OPC_LD_E_D(void);
void OPC_LD_E_E(void);
void OPC_LD_E_H(void);
void OPC_LD_E_L(void);
void OPC_LD_E_HL(void);
void OPC_LD_E_A(void);
void OPC_LD_H_B(void);
void OPC_LD_H_C(void);
void OPC_LD_H_D(void);
void OPC_LD_H_E(void);
void OPC_LD_H_H(void);
void OPC_LD_H_L(void);
void OPC_LD_H_HL(void);
void OPC_LD_H_A(void);
void OPC_LD_L_B(void);
void OPC_LD_L_C(void);
void OPC_LD_L_D(void);
void OPC_LD_L_E(void);
void OPC_LD_L_H(void);
void OPC_LD_L_L(void);
void OPC_LD_L_HL(void);
void OPC_LD_L_A(void);
void OPC_LD_A_B(void);
void OPC_LD_A_C(void);
void OPC_LD_A_D(void);
void OPC_LD_A_E(void);
void OPC_LD_A_H(void);
void OPC_LD_A_L(void);
void OPC_LD_A_HL(void);
void OPC_LD_A_A(void);

uint16_t cpu_get_two_bytes(uint16_t addr);

/******************************************************
 *** 8-BIT ALU                                      ***
 ******************************************************/

void OPC_ADD_A_A(void);

void OPC_ADD_A_B(void);

void OPC_ADD_A_C(void);

void OPC_ADD_A_D(void);

void OPC_ADD_A_E(void);

void OPC_ADD_A_H(void);

void OPC_ADD_A_L(void);

/**
 * @brief First fetches a byte from the address `HL`,
 * 		  then adds the fetched byte to A.
 */
void OPC_ADD_A_HL(void);

/**
 * @brief First fetches an immediate byte from PC + 1,
 * 		  then adds the fetched byte to A.
 *
 * @warning PC cannot be incremented before this operation is completed
 * 			since it reads the data from the opcode itself.
 */
void OPC_ADD_A_d8(void);




void OPC_SUB_A_A(void);

void OPC_SUB_A_B(void);

void OPC_SUB_A_C(void);

void OPC_SUB_A_D(void);

void OPC_SUB_A_E(void);

void OPC_SUB_A_H(void);

void OPC_SUB_A_L(void);

/**
 * @brief First fetches a byte from the address `HL`,
 * 		  then subtracts the fetched byte from A.
 */
void OPC_SUB_A_HL(void);

/**
 * @brief First fetches an immediate byte from PC + 1,
 * 		  then subtracts the fetched byte from A.
 *
 * @warning PC cannot be incremented before this operation is completed
 * 			since it reads the data from the opcode itself.
 */
void OPC_SUB_A_d8(void);

#endif //YOBEMAG_CPU_H
