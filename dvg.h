#ifndef ASTROIDS_DVG_H
#define ASTROIDS_DVG_H

#include "asteroids.h"

#define MIN_X 0
#define MIN_Y 0
#define MAX_X 1023
#define MAX_Y 1023
#define MIN_SCALING_FACTOR 0
#define MAX_SCALING_FACTOR 9

#define DGV_OPCODE_VEC_0 0x0
#define DGV_OPCODE_VEC_1 0x1
#define DGV_OPCODE_VEC_2 0x2
#define DGV_OPCODE_VEC_3 0x3
#define DGV_OPCODE_VEC_4 0x4
#define DGV_OPCODE_VEC_5 0x5
#define DGV_OPCODE_VEC_6 0x6
#define DGV_OPCODE_VEC_7 0x7
#define DGV_OPCODE_VEC_8 0x8
#define DGV_OPCODE_VEC_9 0x9
#define DGV_OPCODE_CUR 0xA
#define DGV_OPCODE_HALT 0xB
#define DGV_OPCODE_JSR 0xC
#define DGV_OPCODE_RTS 0xD
#define DGV_OPCODE_JMP 0xE
#define DGV_OPCODE_SVEC 0xF

void dvg_init(uint16_t start_pc);
void dvg_run();

#endif //ASTROIDS_DVG_H
