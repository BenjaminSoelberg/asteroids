#ifndef ASTROIDS_DVG_H
#define ASTROIDS_DVG_H

#include "asteroids.h"

#define DVG_MIN_X 0
#define DVG_MAX_X 0x3FF
#define DVG_X_MASK 0x3FF
#define DVG_MIN_Y 0
#define DVG_MAX_Y 0x3FF
#define DVG_Y_MASK 0x3FF
#define DVG_MIN_SF 0
#define DVG_MAX_SF 9
#define DVG_SF_MASK 0xF
#define DVG_MIN_SP 0
#define DVG_MAX_SP 3
#define DVG_MIN_PC 0
#define DVG_MAX_PC 0x0FFF
#define DVG_PC_MASK 0x0FFF

#define DVG_OPCODE_MASK 0xF
#define DVG_OPCODE_VEC_0 0x0
#define DVG_OPCODE_VEC_1 0x1
#define DVG_OPCODE_VEC_2 0x2
#define DVG_OPCODE_VEC_3 0x3
#define DVG_OPCODE_VEC_4 0x4
#define DVG_OPCODE_VEC_5 0x5
#define DVG_OPCODE_VEC_6 0x6
#define DVG_OPCODE_VEC_7 0x7
#define DVG_OPCODE_VEC_8 0x8
#define DVG_OPCODE_VEC_9 0x9
#define DVG_OPCODE_CUR 0xA
#define DVG_OPCODE_HALT 0xB
#define DVG_OPCODE_JSR 0xC
#define DVG_OPCODE_RTS 0xD
#define DVG_OPCODE_JMP 0xE
#define DVG_OPCODE_SVEC 0xF

void dvg_init();

void dvg_run();

#endif //ASTROIDS_DVG_H
