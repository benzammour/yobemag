#ifndef YOBEMAG_ALU_H
#define YOBEMAG_ALU_H

typedef struct TestParams {
    uint8_t opcode;
    uint8_t lhs;
    uint8_t rhs;
    size_t rhs_dreg_offset;
    size_t rhs_reg_offset;
    uint8_t expected;
    int address_increment;
    uint8_t F;
} TestParams;

typedef struct SpecialTestParams {
    uint8_t opcode;
    uint8_t lhs;
    uint8_t rhs;
    uint8_t expected;
    uint8_t F;
    bool is_HL;
    bool uses_borrow;
} SpecialTestParams;

void emulate_instruction(TestParams const *params);

void emulate_HL_d8_instruction(SpecialTestParams const *params);

#endif // YOBEMAG_ALU_H
