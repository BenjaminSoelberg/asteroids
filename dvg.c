#include <stdbool.h>
#include "dvg.h"

uint16_t current_x, current_y;
uint16_t scaling_factor;

uint16_t pc;

const uint16_t SCALING_FACTORS[] = {512, 256, 128, 64, 32, 16, 8, 4, 2, 1};

void dvg_parse_cur(uint16_t ins, uint16_t instruction);

void dvg_init(uint16_t start_pc) {
    current_x = MIN_X;
    current_y = MIN_Y;
    scaling_factor = SCALING_FACTORS[MAX_SCALING_FACTOR];
    pc = start_pc;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantFunctionResult"
uint16_t get_next_instruction() {
    uint16_t ins = DGV_OPCODE_HALT << 12;
    pc++;
    return ins;
}
#pragma clang diagnostic pop

void dvg_parse_vec(uint16_t ins_a, uint16_t ins_b) {
}

void dvg_parse_cur(uint16_t ins_a, uint16_t ins_b) {
}

void dvg_parse_halt(uint16_t ins_a) {
}

void dvg_parse_jsr(uint16_t ins_a) {
}

void dvg_parse_rts(uint16_t ins_a) {
}

void dvg_parse_jmp(uint16_t ins_a) {
}

void dvg_parse_svec(uint16_t ins_a) {
}

void dvg_run() {
    bool halt = false;
    while (!halt) {
        uint16_t ins = get_next_instruction();

        switch (ins >> 12 & 0xF) {
            case DGV_OPCODE_VEC_0:
            case DGV_OPCODE_VEC_1:
            case DGV_OPCODE_VEC_2:
            case DGV_OPCODE_VEC_3:
            case DGV_OPCODE_VEC_4:
            case DGV_OPCODE_VEC_5:
            case DGV_OPCODE_VEC_6:
            case DGV_OPCODE_VEC_7:
            case DGV_OPCODE_VEC_8:
            case DGV_OPCODE_VEC_9:
                dvg_parse_vec(ins, get_next_instruction());
                break;

            case DGV_OPCODE_CUR:
                dvg_parse_cur(ins, get_next_instruction());
                break;

            case DGV_OPCODE_HALT:
                dvg_parse_halt(ins);
                halt = true;
                break;

            case DGV_OPCODE_JSR:
                dvg_parse_jsr(ins);
                break;

            case DGV_OPCODE_RTS:
                dvg_parse_rts(ins);
                break;

            case DGV_OPCODE_JMP:
                dvg_parse_jmp(ins);
                break;

            case DGV_OPCODE_SVEC:
                dvg_parse_svec(ins);
                break;

            default:
                halt = true;
        }
    }
}
