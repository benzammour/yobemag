#ifndef CPU_H
#define CPU_H
#include <stdint.h>

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

uint16_t cpu_get_two_bytes(uint16_t addr);

#endif
