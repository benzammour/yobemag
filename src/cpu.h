#ifndef CPU_H
#define CPU_H
#include <stdint.h>

typedef struct cpu {
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
} cpu;

extern cpu c;

void cpu_init(void);
__attribute__((pure)) uint16_t cpu_get_cycle_count(void);
uint8_t cpu_step(void);

__attribute__((pure)) uint16_t get_AF(void);
__attribute__((pure)) uint16_t get_BC(void);
__attribute__((pure)) uint16_t get_DE(void);
__attribute__((pure)) uint16_t get_HL(void);
__attribute__((pure)) uint8_t get_Z(void);
__attribute__((pure)) uint8_t get_N(void);
__attribute__((pure)) uint8_t get_H(void);
__attribute__((pure)) uint8_t get_C(void);
__attribute__((pure)) uint16_t cpu_get_PC(void);
void cpu_print_registers(void);
void set_Z(uint8_t bit);
void set_N(uint8_t bit);
void set_H(uint8_t bit);
void set_C(uint8_t bit);
void LD_REG_REG(uint8_t *register_one, uint8_t register_two);
void LD_8(uint8_t *addr);
void REG_XOR(uint8_t *register_x, uint8_t register_y);
void REG_DEC(uint8_t *reg);
void REG_INC(uint8_t *reg);
void set_AF(uint16_t x);
void set_BC(uint16_t x);
void set_DE(uint16_t x);
void set_HL(uint16_t x);

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
void OPC_LD_HL_A(void);
void OPC_LD_HL_B(void);
void OPC_LD_HL_C(void);
void OPC_LD_HL_D(void);
void OPC_LD_HL_E(void);
void OPC_LD_HL_H(void);
void OPC_LD_HL_L(void);
void OPC_HALT(void);
void OPC_LD_A_B(void);
void OPC_LD_A_C(void);
void OPC_LD_A_D(void);
void OPC_LD_A_E(void);
void OPC_LD_A_H(void);
void OPC_LD_A_L(void);
void OPC_LD_A_HL(void);
void OPC_LD_A_A(void);

uint16_t cpu_get_two_bytes(uint16_t addr);

#endif
