/**
 *
 * @file	registers.h
 * @brief 	Contains defintion of all special registers as well as macros to access relevant bit fields
 * @author 	Moritz R. Huellmann
 * @date 	17.10.22
 *
 */

#ifndef YOBEMAG_SRC_REGISTERS_H
#define YOBEMAG_SRC_REGISTERS_H

#include <stdint.h>

typedef struct SReg8 {
    uint8_t value;
} SReg8;

typedef union SReg16 {
    uint8_t words[2];
    uint16_t value;
} SReg16;

typedef union SReg32 {
    uint8_t words[4];
    uint32_t value;
} SReg32;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-macros"

#define BIT_POS_0 (0U)
#define BIT_POS_1 (1U)
#define BIT_POS_2 (2U)
#define BIT_POS_3 (3U)
#define BIT_POS_4 (4U)
#define BIT_POS_5 (5U)
#define BIT_POS_6 (6U)
#define BIT_POS_7 (7U)

#define BIT_MASK_0 (0x01U)
#define BIT_MASK_1 (0x02U)
#define BIT_MASK_2 (0x04U)
#define BIT_MASK_3 (0x08U)
#define BIT_MASK_4 (0x10U)
#define BIT_MASK_5 (0x20U)
#define BIT_MASK_6 (0x40U)
#define BIT_MASK_7 (0x80U)

#define BIT_VALUE_00 (0x00U)
#define BIT_VALUE_01 (0x01U)
#define BIT_VALUE_10 (0x02U)
#define BIT_VALUE_11 (0x03U)

#define GET_BITS(word, pos, mask)      (((uint8_t)word >> (uint8_t)pos) & (uint8_t)mask)
#define SET_BITS(word, pos, mask, val) (word = ((uint8_t)word & ~(uint8_t)mask) | ((uint8_t)val << (uint8_t)pos))

/******************************************************
 *** SpecialRegister sregs                          ***
 ******************************************************/

typedef struct SpecialRegisters {
    SReg8 p1;
} SpecialRegisters;

extern SpecialRegisters sregs;

/******************************************************
 *** FF00 (P1)                                      ***
 ******************************************************/

#define REG_P1_GET_P15() GET_BITS(sregs.p1, BIT_POS_5, BIT_MASK_5);
#define REG_P1_GET_P14() GET_BITS(sregs.p1, BIT_POS_4, BIT_MASK_4);
#define REG_P1_GET_P13() GET_BITS(sregs.p1, BIT_POS_3, BIT_MASK_3);
#define REG_P1_GET_P12() GET_BITS(sregs.p1, BIT_POS_2, BIT_MASK_2);
#define REG_P1_GET_P11() GET_BITS(sregs.p1, BIT_POS_1, BIT_MASK_1);
#define REG_P1_GET_P10() GET_BITS(sregs.p1, BIT_POS_0, BIT_MASK_0);

#define REG_P1_SET_P15(val) SET_BITS(sregs.p1, BIT_POS_5, BIT_MASK_5, val);
#define REG_P1_SET_P14(val) SET_BITS(sregs.p1, BIT_POS_4, BIT_MASK_4, val);
#define REG_P1_SET_P13(val) SET_BITS(sregs.p1, BIT_POS_3, BIT_MASK_3, val);
#define REG_P1_SET_P12(val) SET_BITS(sregs.p1, BIT_POS_2, BIT_MASK_2, val);
#define REG_P1_SET_P11(val) SET_BITS(sregs.p1, BIT_POS_1, BIT_MASK_1, val);
#define REG_P1_SET_P10(val) SET_BITS(sregs.p1, BIT_POS_0, BIT_MASK_0, val);

#pragma clang diagnostic pop

#endif /* YOBEMAG_SRC_REGISTERS_H */
