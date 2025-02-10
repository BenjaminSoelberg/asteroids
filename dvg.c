#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "dvg.h"

uint16_t x;
uint16_t y;
uint16_t gsf;

uint16_t pc;
uint8_t sp;

uint16_t stack[DVG_MAX_SP + 1];
const uint16_t SCALING_FACTORS[] = {512, 256, 128, 64, 32, 16, 8, 4, 2, 1};

void dvg_init() {
    x = DVG_MIN_X;
    y = DVG_MIN_Y;
    gsf = DVG_MAX_SF;
    memset(&stack, 0, sizeof stack);
    sp = DVG_MIN_SP;
    pc = DVG_MIN_PC;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantFunctionResult"

uint16_t get_next_word() {
    //TODO Assert min max pc
    uint16_t word = DVG_OPCODE_HALT << 12;
    pc++;
    return word;
}

#pragma clang diagnostic pop

void dvg_parse_vec(uint16_t word_1, uint16_t word_2) {
    //TODO: Remember to implement
}

void dvg_parse_cur(uint16_t word_1, uint16_t word_2) {
    uint16_t new_y = word_1 & DVG_Y_MASK;
    assert(new_y < DVG_MAX_Y);

    uint16_t new_x = word_2 & DVG_X_MASK;
    assert(new_x < DVG_MAX_X);

    uint16_t new_gsf = word_2 >> 12 & DVG_SF_MASK;
    assert(new_gsf <= DVG_MAX_SF);

    y = new_y;
    x = new_x;
    gsf = new_gsf;
}

void dvg_parse_halt() {
    //TODO: set pc to start_pc ?
    //TODO: Remember to implement
}

void dvg_parse_jsr(uint16_t word) {
    assert(sp < DVG_MAX_SP);
    uint8_t new_sp = sp + 1;

    uint16_t new_pc = word & DVG_PC_MASK;
    assert(new_pc <= DVG_MAX_PC);

    stack[sp] = pc;
    sp = new_sp;
    pc = new_pc;
}

void dvg_parse_rts() {
    assert(sp > DVG_MIN_SP);
    uint8_t new_sp = sp - 1;

    uint16_t new_pc = stack[sp];
    assert(new_pc <= DVG_MAX_PC);

    sp = new_sp;
    pc = new_pc;
}

void dvg_parse_jmp(uint16_t word) {
    uint16_t new_pc = word & DVG_PC_MASK;
    assert(new_pc <= DVG_MAX_PC);

    pc = new_pc;
}

void dvg_parse_svec(uint16_t word) {
    //TODO: Remember to implement
}

void dvg_run() {
    bool keep_running = true;
    do {
        uint16_t word = get_next_word();

        int opcode = word >> 12 & DVG_OPCODE_MASK;
        switch (opcode) {
            case DVG_OPCODE_VEC_0:
            case DVG_OPCODE_VEC_1:
            case DVG_OPCODE_VEC_2:
            case DVG_OPCODE_VEC_3:
            case DVG_OPCODE_VEC_4:
            case DVG_OPCODE_VEC_5:
            case DVG_OPCODE_VEC_6:
            case DVG_OPCODE_VEC_7:
            case DVG_OPCODE_VEC_8:
            case DVG_OPCODE_VEC_9:
                dvg_parse_vec(word, get_next_word());
                break;

            case DVG_OPCODE_CUR:
                dvg_parse_cur(word, get_next_word());
                break;

            case DVG_OPCODE_HALT:
                dvg_parse_halt();
                keep_running = false;
                break;

            case DVG_OPCODE_JSR:
                dvg_parse_jsr(word);
                break;

            case DVG_OPCODE_RTS:
                dvg_parse_rts();
                break;

            case DVG_OPCODE_JMP:
                dvg_parse_jmp(word);
                break;

            case DVG_OPCODE_SVEC:
                dvg_parse_svec(word);
                break;

            default:
                assert(0);
        }
    } while (keep_running);
}
