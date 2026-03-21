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

#ifndef ASTEROIDS_DEBUG
#define ASTEROIDS_DEBUG 0
#endif

void debug_printf(const char *format, ...) {
#if ASTEROIDS_DEBUG
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
#else
    (void)format;
#endif
}

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

int16_t scale(int16_t value, int8_t sf) {
    return sf >= 9 ? value << sf - 9 : value >> 9 - sf;
}

void dvg_parse_vctr(cairo_t *cr, uint16_t word_1, uint16_t word_2) {
    const uint8_t lsf = (word_1 & VCTR_W1_SF_MASK) >> 12;
    assert(lsf <= DVG_MAX_SF);

    const bool y_sign = (word_1 & VCTR_W1_SIGN_MASK) >> 10;

    int16_t delta_y = (int16_t) (word_1 & VCTR_W1_Y_MASK);
    assert(delta_y <= DVG_MAX_Y);

    const uint8_t brightness = (word_2 & VCTR_W2_BRIGHTNESS_MASK) >> 12;
    assert(brightness <= DVG_MAX_BRIGHTNESS);

    const bool x_sign = (word_2 & VCTR_W2_SIGN_MASK) >> 10;

    int16_t delta_x = (int16_t) (word_2 & VCTR_W2_X_MASK);
    assert(delta_x <= DVG_MAX_X);

    delta_y = y_sign ? -delta_y : delta_y; // One's complement
    delta_x = x_sign ? -delta_x : delta_x; // One's complement

    int8_t sf = (int8_t)((gsf + lsf) & 0xF);

    int16_t scaled_delta_y = scale(delta_y, sf);
    int16_t scaled_delta_x = scale(delta_x, sf);

    debug_printf("0x%04X VCTR gsf=%d lsf=%d sf=%d bri=%d x=%d y=%d -> (%d, %d)\n",
           current_pc, gsf, lsf, sf, brightness, delta_x, delta_y,
           scaled_delta_x, scaled_delta_y);

    dvg_draw_to(cr, scaled_delta_x, scaled_delta_y, brightness);
}

void dvg_parse_labs(uint16_t word_1, uint16_t word_2) {
    // According to some docs, X/Y should be signed, but that doesn't make sense unless you move 0,0 to the center of the screen
    assert((word_1 & 0b10000000000) == 0);
    assert((word_2 & 0b10000000000) == 0);

    uint16_t new_y = word_1 & LABS_W1_Y_MASK;
    assert(new_y <= DVG_MAX_Y);

    uint16_t new_x = word_2 & LABS_W2_X_MASK;
    assert(new_x <= DVG_MAX_X);

    uint8_t new_gsf = (word_2 & LABS_W2_SF_MASK) >> 12;
    assert(new_gsf <= DVG_MAX_SF);

    debug_printf("0x%04X LABS scale=%d, x=%d, y=%d\n", current_pc, new_gsf, new_x, new_y);

    y = new_y;
    x = new_x;
    gsf = new_gsf;
}

void dvg_parse_halt() {
    debug_printf("0x%04X HALT\n", current_pc);
}

void dvg_parse_jsrl(uint16_t word) {
    assert(sp < DVG_MAX_SP);
    uint8_t new_sp = sp + 1;

    uint16_t new_pc = word & JSRL_PC_MASK;
    assert(new_pc >= DVG_MIN_PC);
    assert(new_pc <= DVG_MAX_PC);

    debug_printf("0x%04X JSRL sp=0x%02X 0x%04X\n", current_pc, new_sp, new_pc);

    stack[sp] = pc;
    sp = new_sp;
    pc = new_pc;
}

void dvg_parse_rtsl() {
    assert(sp > DVG_MIN_SP);
    assert(sp <= DVG_MAX_SP);
    uint8_t new_sp = sp - 1;

    uint16_t new_pc = stack[new_sp];
    assert(new_pc >= DVG_MIN_PC);
    assert(new_pc <= DVG_MAX_PC);

    debug_printf("0x%04X RTSL sp=0x%02X 0x%04X\n", current_pc, new_sp, new_pc);
    sp = new_sp;
    pc = new_pc;
}

void dvg_parse_jmpl(uint16_t word) {
    uint16_t new_pc = word & JMPL_PC_MASK;
    assert(new_pc >= DVG_MIN_PC);
    assert(new_pc <= DVG_MAX_PC);

    debug_printf("0x%04X JMPL 0x%04X\n", current_pc, new_pc);
    pc = new_pc;
}

void dvg_parse_svec(cairo_t *cr, uint16_t word) {
    uint8_t ss = (word & SVEC_S1_MASK) >> 11 | (word & SVEC_S0_MASK) >> 2;
    assert(ss <= SVEC_MAX_SS);

    uint8_t brightness = (word & SVEC_BRIGHTNESS_MASK) >> 4;
    assert(brightness <= DVG_MAX_BRIGHTNESS);

    bool y_sign = (word & SVEC_Y_SIGN_MASK) >> 10;

    int16_t delta_y = (word & SVEC_Y_MASK) >> 8;
    assert(delta_y <= SVEC_MAX_Y);

    bool x_sign = (word & SVEC_X_SIGN_MASK) >> 2;

    int16_t delta_x = (word & SVEC_X_MASK) >> 0;
    assert(delta_x <= SVEC_MAX_X);

    uint8_t lsf = ss + 2; // Normalize to match scaling scale

    delta_y = y_sign ? -delta_y : delta_y; // One's complement
    delta_x = x_sign ? -delta_x : delta_x; // One's complement

    int8_t sf = (int8_t)((gsf + (8 + lsf)) & 0xF);

    int16_t scaled_delta_y = scale(delta_y, sf);
    int16_t scaled_delta_x = scale(delta_x, sf);

    debug_printf("0x%04X SVEC gsf=%d lsf=%d sf=%d bri=%d x=%d y=%d -> (%d, %d)\n",
           current_pc, gsf, lsf, sf, brightness, delta_x, delta_y,
           scaled_delta_x, scaled_delta_y);

    dvg_draw_to(cr, scaled_delta_x, scaled_delta_y, brightness);
}

void dvg_run(cairo_t *cr, uint16_t start_pc) {
    bool keep_running = true;
    pc = start_pc;
    do {
        current_pc = pc;
        uint16_t word = get_next_word();

        int opcode = (word & DVG_OPCODE_MASK) >> 12;
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

            case DVG_OPCODE_JSRL:
                dvg_parse_jsrl(word);
                break;

            case DVG_OPCODE_RTSL:
                dvg_parse_rtsl();
                break;

            case DVG_OPCODE_JMPL:
                dvg_parse_jmpl(word);
                break;

            case DVG_OPCODE_SVEC:
                dvg_parse_svec(cr, word);
                break;

            default:
                assert(0);
        }

    } while (keep_running);
}
