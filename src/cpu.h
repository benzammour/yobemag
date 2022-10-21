#ifndef YOBEMAG_CPU_H
#define YOBEMAG_CPU_H

#include <stdint.h>

/**
 * Encodes bit positions for flag register A
 */
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

typedef union DoubleWordReg {
    struct {
        uint8_t hi; // A, B, D, H
        uint8_t lo; // F, C, E, L
    } words;
    uint16_t dword;
} DoubleWordReg;

typedef struct CPU {
    DoubleWordReg HL;
    DoubleWordReg DE;
    DoubleWordReg BC;
    DoubleWordReg AF;

    uint16_t SP;
    uint16_t PC;

    uint16_t cycle_count;

    uint8_t opcode;
} CPU;

extern CPU cpu;

#define CPU_DREG_HL cpu.HL.dword
#define CPU_REG_H   cpu.HL.words.hi
#define CPU_REG_L   cpu.HL.words.lo

#define CPU_DREG_DE cpu.DE.dword
#define CPU_REG_D   cpu.DE.words.hi
#define CPU_REG_E   cpu.DE.words.lo

#define CPU_DREG_BC cpu.BC.dword
#define CPU_REG_B   cpu.BC.words.hi
#define CPU_REG_C   cpu.BC.words.lo

#define CPU_DREG_AF cpu.AF.dword
#define CPU_REG_A   cpu.AF.words.hi
#define CPU_REG_F   cpu.AF.words.lo

void cpu_init(void);
void cpu_step(void);

void cpu_print_registers(void);

__attribute((always_inline)) inline uint8_t get_flag(Flag f) {
    return (CPU_REG_F >> f) & 1;
}

__attribute((always_inline)) inline void set_flag(uint8_t bit, Flag f) {
    CPU_REG_F |= bit << f;
}

__attribute((always_inline)) inline void clear_flag(Flag f) {
    CPU_REG_F &= ~(1 << f);
}

void LD_8(uint8_t *addr);
void REG_XOR(uint8_t *register_x, uint8_t register_y);
void REG_DEC(uint8_t *reg);
void REG_INC(uint8_t *reg);

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

// 8-bit ALU: ADD A,n
void OPC_ADD_A_A(void);

void OPC_ADD_A_B(void);

void OPC_ADD_A_C(void);

void OPC_ADD_A_D(void);

void OPC_ADD_A_E(void);

void OPC_ADD_A_H(void);

void OPC_ADD_A_L(void);

/**
 * @brief First fetches a byte from the address `HL`,
 * 	      then adds the fetched byte to A.
 *
 * @note This instruction requires the PC to be incremented by 1,
 *       but takes 8 CPU cycles because of fetching a byte from memory
 */
void OPC_ADD_A_HL(void);

/**
 * @brief First fetches an immediate byte from PC + 1,
 * 	      then adds the fetched byte to A.
 *
 * @warning PC cannot be incremented before this operation is completed
 * 			since it reads the data from the opcode itself.
 *
 * @note This instruction requires the PC to be incremented by 2.
 */
void OPC_ADD_A_d8(void);

/**
 * @brief Subtract the value stored in Register A from
 * 	      the value stored in A and store it back into A.
 */
void OPC_SUB_A_A(void);

/**
 * @brief Subtract the value stored in Register B from
 * 	      the value stored in A and store it back into A.
 */
void OPC_SUB_A_B(void);

/**
 * @brief Subtract the value stored in Register C from
 * 	      the value stored in A and store it back into A.
 */
void OPC_SUB_A_C(void);

/**
 * @brief Subtract the value stored in Register D from
 * 	      the value stored in A and store it back into A.
 */
void OPC_SUB_A_D(void);

/**
 * @brief Subtract the value stored in Register E from
 * 	      the value stored in A and store it back into A.
 */
void OPC_SUB_A_E(void);

/**
 * @brief Subtract the value stored in Register H from
 * 	      the value stored in A and store it back into A.
 */
void OPC_SUB_A_H(void);

/**
 * @brief Subtract the value stored in Register L from
 * 	      the value stored in A and store it back into A.
 */
void OPC_SUB_A_L(void);

/**
 * @brief First fetches a byte from the address `HL`,
 * 	      then subtracts the fetched byte from A.
 *        As this operation fetches a byte from memory, this takes 8 cycles.
 */
void OPC_SUB_A_HL(void);

/**
 * @brief First fetches an immediate byte from PC + 1,
 * 	      then subtracts the fetched byte from A.
 *
 * @warning PC cannot be incremented before this operation is completed
 * 			since it reads the data from the opcode itself.
 *          As this operation fetches a byte from memory, this takes 8 cycles.
 */
void OPC_SUB_A_d8(void);

// 8-bit ALU: ADC A,n
void OPC_ADC_A_A(void);

void OPC_ADC_A_B(void);

void OPC_ADC_A_C(void);

void OPC_ADC_A_D(void);

void OPC_ADC_A_E(void);

void OPC_ADC_A_H(void);

void OPC_ADC_A_L(void);

/**
 * @brief First fetches a byte from the address `HL`,
 * 		  then adds the fetched byte to A together with
 * 		  the carry flag.
 *
 * @note This instruction requires the PC to be incremented by 1,
 *       but takes 8 CPU cycles because of fetching a byte from memory.
 */
void OPC_ADC_A_HL(void);

/**
 * @brief First fetches an immediate byte from PC + 1,
 * 		  then adds the fetched byte to A together with
 * 		  the carry flag.
 *
 * @warning PC cannot be incremented before this operation
 * 			is completed since it reads the data from the
 * 			opcode itself.
 *
 * @note This instruction requires the PC to be incremented by 2.
 */
void OPC_ADC_A_d8(void);

/**
 * @brief Subtract the value stored in Register A from the value stored in A,
 * 	      additionally subtract the carry flag, then store it in A.
 */
void OPC_SBC_A_A(void);

/**
 * @brief Subtract the value stored in Register B from the value stored in A,
 *        additionally subtract the carry flag, then store it in A.
 */
void OPC_SBC_A_B(void);

/**
 * @brief Subtract the value stored in Register C from the value stored in A,
 *        addtionally, subtract the carry flag, then store it in A.
 */
void OPC_SBC_A_C(void);

/**
 * @brief Subtract the value stored in Register D from the value stored in A,
 *        addtionally, subtract the carry flag, then store it in A.
 */
void OPC_SBC_A_D(void);

/**
 * @brief Subtract the value stored in Register E from the value stored in A,
 *        addtionally, subtract the carry flag, then store it in A.
 */
void OPC_SBC_A_E(void);

/**
 * @brief Subtract the value stored in Register H from the value stored in A,
 *        addtionally, subtract the carry flag, then store it in A.
 */
void OPC_SBC_A_H(void);

/**
 * @brief Subtract the value stored in Register L from the value stored in A,
 *        addtionally, subtract the carry flag, then store it in A.
 */
void OPC_SBC_A_L(void);

/**
 * @brief First fetches a byte from the address `HL`, subtracts the fetched byte from A
 *        addtionally, subtract the carry flag, then store it in A.
 *        As this operation fetches a byte from memory, this takes 8 cycles.
 */
void OPC_SBC_A_HL(void);

/**
 * @brief First fetches an immediate byte from PC + 1, subtracts the fetched byte from A
 *        addtionally, subtract the carry flag, then store it in A.
 *
 * @warning PC cannot be incremented before this operation is completed
 * 			since it reads the data from the opcode itself.
 *          As this operation fetches a byte from memory, this takes 8 cycles.
 */
void OPC_SBC_A_d8(void);

#endif // YOBEMAG_CPU_H
