#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "commons.h"
#include "dvg.h"

uint16_t x;
uint16_t y;
uint8_t gsf;

uint16_t pc, current_pc;
uint8_t sp;

uint16_t stack[DVG_MAX_SP + 1];

//TODO: Change all SHIFT + AND to AND + SHIFT as it makes more sense

void dvg_init() {
    x = DVG_MIN_X;
    y = DVG_MIN_Y;
    gsf = DVG_MAX_SF;
    memset(&stack, 0, sizeof stack);
    sp = DVG_MIN_SP;
    pc = 0x0000;
    current_pc = pc;
}

uint16_t get_next_word() {
    uint16_t word = memory.VECMEM_16[pc];
    pc++;
    return word;
}

int16_t scale(int16_t value) {
    return gsf >= 8 ? value >> (16 - gsf) : value << gsf;
}

void dvg_draw_to(cairo_t *cr, int16_t delta_x, int16_t delta_y, uint8_t brightness) {
    uint16_t new_x = x + delta_x;
    uint16_t new_y = y + delta_y;

    assert(brightness <= DVG_MAX_BRIGHTNESS);

    if (brightness > DVG_MIN_BRIGHTNESS) {
        cairo_set_source_rgb(cr, 1.0 / DVG_MAX_BRIGHTNESS * brightness, 1.0 / DVG_MAX_BRIGHTNESS * brightness, 1.0 / DVG_MAX_BRIGHTNESS * brightness);
        cairo_set_line_width(cr, 1);

        cairo_move_to(cr, x, DVG_MAX_Y - y);
        cairo_line_to(cr, new_x, DVG_MAX_Y - new_y);
        cairo_stroke(cr);
    }

    x = new_x;
    y = new_y;
}

void dvg_parse_vctr(cairo_t *cr, uint16_t word_1, uint16_t word_2) {
    const uint8_t sf = word_1 >> 12 & DVG_SF_MASK;
    assert(sf <= DVG_MAX_SF);

    const bool y_sign = word_1 >> 10 & DVG_SIGN_MASK;

    int16_t delta_y = (int16_t) (word_1 & DVG_Y_MASK);
    assert(delta_y <= DVG_MAX_Y);

    const uint8_t brightness = word_2 >> 12 & DVG_BRIGHTNESS_MASK;
    assert(brightness <= DVG_MAX_BRIGHTNESS);

    const bool x_sign = word_2 >> 10 & DVG_SIGN_MASK;

    int16_t delta_x = (int16_t) (word_2 & DVG_X_MASK);
    assert(delta_x <= DVG_MAX_X);

    delta_y = y_sign ? -delta_y : delta_y; // One's complement
    delta_x = x_sign ? -delta_x : delta_x; // One's complement
    const int16_t scaled_delta_y = delta_y >> (9 - sf);
    const int16_t scaled_delta_x = delta_x >> (9 - sf);

    printf("0x%04X VCTR scale=%d, bri=%d, x=%d, y=%d  (%d, %d)\n", current_pc, 0, brightness, delta_x, delta_y,
           scaled_delta_x, scaled_delta_y);

    dvg_draw_to(cr, scaled_delta_x, scaled_delta_y, brightness);
}

void dvg_parse_labs(uint16_t word_1, uint16_t word_2) {
    assert((word_1 & 0b10000000000) == 0); // According to some docs Y should be signed but that doesn't make sense unless you move 0,0 to the center of the screen
    assert((word_2 & 0b10000000000) == 0); // According to some docs X should be signed but that doesn't make sense unless you move 0,0 to the center of the screen

    uint16_t new_y = word_1 & DVG_Y_MASK;
    assert(new_y <= DVG_MAX_Y);

    uint16_t new_x = word_2 & DVG_X_MASK;
    assert(new_x <= DVG_MAX_X);

    uint8_t new_gsf = word_2 >> 12 & DVG_SF_MASK;
    assert(new_gsf <= DVG_MAX_SF);

    printf("0x%04X LABS scale=%d, x=%d, y=%d\n", current_pc, new_gsf, new_x, new_y);

    y = new_y;
    x = new_x;
    gsf = new_gsf;
}

void dvg_parse_halt() {
    printf("0x%04X HALT \n", current_pc);
}

void dvg_parse_jsr(uint16_t word) {
    assert(sp < DVG_MAX_SP);
    uint8_t new_sp = sp + 1;

    uint16_t new_pc = word & DVG_PC_MASK;
    assert(new_pc >= DVG_MIN_PC);
    assert(new_pc <= DVG_MAX_PC);

    printf("0x%04X JSR sp=0x%02X 0x%04X\n", current_pc, new_sp, new_pc);

    stack[sp] = pc;
    sp = new_sp;
    pc = new_pc;
}

void dvg_parse_rts() {
    assert(sp > DVG_MIN_SP);
    assert(sp <= DVG_MAX_SP);
    uint8_t new_sp = sp - 1;

    uint16_t new_pc = stack[new_sp];
    assert(new_pc >= DVG_MIN_PC);
    assert(new_pc <= DVG_MAX_PC);

    printf("0x%04X RTC sp=0x%02X 0x%04X \n", current_pc, new_sp, new_pc);
    sp = new_sp;
    pc = new_pc;
}

void dvg_parse_jmp(uint16_t word) {
    uint16_t new_pc = word & DVG_PC_MASK;
    assert(new_pc >= DVG_MIN_PC);
    assert(new_pc <= DVG_MAX_PC);

    printf("0x%04X JMP 0x%04X\n", current_pc, new_pc);
    pc = new_pc;
}

void dvg_parse_svec(cairo_t *cr, uint16_t word) {
    uint8_t ss = (word >> 11 & DVG_S0_MASK) | (word >> 2 & DVG_S1_MASK);
    assert(ss <= DVG_MAX_SS);

    uint8_t brightness = word >> 4 & DVG_BRIGHTNESS_MASK;
    assert(brightness <= DVG_MAX_BRIGHTNESS);

    bool y_sign = word >> 10 & DVG_SIGN_MASK;

    int16_t delta_y = (word >> 8) & DVG_YY_MASK;
    assert(delta_y <= DVG_MAX_YY);

    bool x_sign = word >> 2 & DVG_SIGN_MASK;

    int16_t delta_x = (word >> 0) & DVG_XX_MASK;
    assert(delta_x <= DVG_MAX_XX);

    uint8_t lsf = 1 << (7 - ss);

    delta_y = delta_y << 8 >> (7 - ss);
    delta_x = delta_x << 8 >> (7 - ss);
    delta_y = (int16_t) (y_sign ? -delta_y : delta_y);
    delta_x = (int16_t) (x_sign ? -delta_x : delta_x);

    int16_t scaled_delta_y = scale(delta_y);
    int16_t scaled_delta_x = scale(delta_x);

    printf("0x%04X SVEC scale=%d(*%d), bri=%d, x=%d, y=%d  (%d, %d)\n", current_pc, ss, lsf, brightness, delta_x, delta_y, scaled_delta_x, scaled_delta_y);

    dvg_draw_to(cr, scaled_delta_x, scaled_delta_y, brightness);
}

void dvg_run(cairo_t *cr, uint16_t start_pc) {
    bool keep_running = true;
    pc = start_pc;
    do {
        current_pc = pc;
        uint16_t word = get_next_word();

        int opcode = word >> 12 & DVG_OPCODE_MASK;
        switch (opcode) {
            case DVG_OPCODE_VCTR_0:
            case DVG_OPCODE_VCTR_1:
            case DVG_OPCODE_VCTR_2:
            case DVG_OPCODE_VCTR_3:
            case DVG_OPCODE_VCTR_4:
            case DVG_OPCODE_VCTR_5:
            case DVG_OPCODE_VCTR_6:
            case DVG_OPCODE_VCTR_7:
            case DVG_OPCODE_VCTR_8:
            case DVG_OPCODE_VCTR_9:
                dvg_parse_vctr(cr, word, get_next_word());
                break;

            case DVG_OPCODE_LABS:
                dvg_parse_labs(word, get_next_word());
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
                dvg_parse_svec(cr, word);
                break;

            default:
                assert(0);
        }

    } while (keep_running);
}
