#include "commons.h"

MEMORY memory;

uint16_t VGMSGA[] = {
        JSRL | CHAR_SPACE, JSRL | CHAR_0, JSRL | CHAR_1, JSRL | CHAR_2,
        JSRL | CHAR_3, JSRL | CHAR_4, JSRL | CHAR_5, JSRL | CHAR_6,
        JSRL | CHAR_7, JSRL | CHAR_8, JSRL | CHAR_9, JSRL | CHAR_A,
        JSRL | CHAR_B, JSRL | CHAR_C, JSRL | CHAR_D, JSRL | CHAR_E,
        JSRL | CHAR_F, JSRL | CHAR_G, JSRL | CHAR_H, JSRL | CHAR_I,
        JSRL | CHAR_J, JSRL | CHAR_K, JSRL | CHAR_L, JSRL | CHAR_M,
        JSRL | CHAR_N, JSRL | CHAR_O, JSRL | CHAR_P, JSRL | CHAR_Q,
        JSRL | CHAR_R, JSRL | CHAR_S, JSRL | CHAR_T, JSRL | CHAR_U,
        JSRL | CHAR_V, JSRL | CHAR_W, JSRL | CHAR_X, JSRL | CHAR_Y,
        JSRL | CHAR_Z
};

int32_t max(int32_t a, int32_t b) {
    return ((a) > (b) ? a : b);
}

int32_t min(int32_t a, int32_t b) {
    return ((a) < (b) ? a : b);
}

/**
 * Used to set OUT1 to allow the bank switching to take place.
 * @param out1 value
 */
void io_setOUT1(uint8_t out1) {
    memory.io._OUT1 = out1;
    //TODO: Not implement, handle bank select ?
}

/**
 * Will wait until the DVG has executed a HALT instruction.
 */
void todo_wait_for_HALT() {
    //    10$:	LDA A,HALT
    //    BMI 10$			;WAIT FOR BEAM TO HALT
    // TODO: Remember to implement
}

void _trigger_nmi() {
    //TODO: Remember to implement
    assert(false);
}

/**
 * I guess any value will start the vector generator
 */
void todo_io_startGOADD() {
    // TODO Clear the HALT flag ?
    //    STA A,GOADD		;START VECTOR GENERATOR
    // TODO: Remember to implement
}

/**
 * I guess any value will poll the watchdog timer
 */
void todo_io_pollWTDOG() {
    //    STA A,WTDOG
    // TODO: Remember to implement
}

/**
 * EXPLOSION SOUND  D6-D7=PITCH,D2-D5=VOLUME
 * @param value
 */
void io_set_EXPSND(uint8_t value) {
    // TODO: Remember to implement
}

/**
 * THUMP SOUND D4=1 FOR ON, D0-D3=FREQUENCY
 * @param value
 */
void io_set_THUMP(uint8_t value) {
    // TODO: Remember to implement
}

/**
 * SAUCER THRUST SOUND,D7=1 FOR ON
 * @param value
 */
void io_set_SCRSND(uint8_t value) {
    // TODO: Remember to implement
}

/**
 * SAUCER FIRE SOUND, D7=1 FOR ON
 * @param value
 */
void io_set_SCRFIR(uint8_t value) {
    // TODO: Remember to implement
}

/**
 * SHIP THRUST SOUND, D7=1 FOR ON
 * @param value
 */
void io_set_SHPSND(uint8_t value) {
    // TODO: Remember to implement
}

/**
 * SHIP FIRE SOUND,D7=1 FOR ON
 * @param value
 */
void io_set_SHPFIR(uint8_t value) {
    // TODO: Remember to implement
}

/**
 * TONE FOR EXTRA LIFE, D7=1 FOR ON
 * @param value 
 */
void io_set_TONE(uint8_t value) {
    // TODO: Remember to implement
}

/**
 * NOISE RESET
 * @param value
 */
void io_set_NRESET(uint8_t value) {
    // TODO: Remember to implement
}

/**
 * Checks if the START 1 button is pressed.
  * @return true if pressed
 */
bool io_is_STRT1_pressed() {
    return ((int8_t)memory.io.STRT1) < 0;
}

/**
 * Checks if the START 1 button is pressed.
  * @return true if pressed
 */
bool io_is_STRT2_pressed() {
    return ((int8_t)memory.io.STRT2) < 0;
}

/**
 * Places a byte in VECRAM at index VGLIST + delta
 *
 * @param Y_delta
 * @param A_value
 */
void vg_memory_put(uint8_t Y_delta, uint8_t A_value) {
    uint16_t index = memory.page0.VGLIST_16 + Y_delta;
    assert(index < sizeof memory.VECMEM_16);
    memory.VECMEM[index] = A_value;
}

/**
 * Places a word in VECRAM at index VGLIST + delta
 *
 * @note Indexes are asserted to be on a word boundary.
 *
 * @param Y_delta
 * @param AX_value
 */
void vg_memory_put16(uint8_t Y_delta, uint16_t AX_value) {
    // I don't expect any word writes across word boundaries.
    assert(memory.page0.VGLIST_16 % 2 == 0); // Assert that no out of word bound writes
    assert(Y_delta % 2 == 0); // Assert that no out of word bound writes

    uint16_t index = (memory.page0.VGLIST_16 + Y_delta) >> 1;
    memory.VECMEM_16[index] = AX_value;
}
