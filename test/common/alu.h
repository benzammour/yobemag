#ifndef YOBEMAG_ALU_H
#define YOBEMAG_ALU_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct ALUTestParams {
    uint8_t opcode;
    uint16_t lhs;
    uint16_t rhs;
    size_t rhs_dreg_offset;
    size_t rhs_reg_offset;
    uint16_t expected;
    int address_increment;
    uint8_t F;
} ALUTestParams;

typedef struct ALUSpecialTestParams {
    uint8_t opcode;
    uint8_t lhs;
    uint8_t rhs;
    uint8_t expected;
    uint8_t F;
    bool is_HL;
    bool uses_borrow;
} ALUSpecialTestParams;

void alu_emulate_8bit_instruction(const ALUTestParams *const params);

void alu_emulate_8bit_HL_d8_instruction(const ALUSpecialTestParams *const params);

void alu_emulate_16bit_instruction(const ALUTestParams *const params);

#endif // YOBEMAG_ALU_H
