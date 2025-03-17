#include "commons.h"

MEMORY memory;

/**
 * Used to set OUT1 to allow the bank switching to take place.
 * @param out1 value
 */
 void io_setOUT1(uint8_t out1) {
    memory.io._OUT1 = out1;
    //TODO: Not implement, handle bank select ?
}

void wait_for_HALT() {
    //    10$:	LDA A,HALT
    //    BMI 10$			;WAIT FOR BEAM TO HALT
    // TODO: Not implemented
}

void trigger_nmi() {
    /* TODO: Not implemented yet */
    assert(false);
}

/**
 * I guess any value will start the vector generator
 */
void io_startGOADD() {
    //    STA A,GOADD		;START VECTOR GENERATOR
    // TODO: Not implemented
}

/**
 * I guess any value will poll the watchdog timer
 */
void io_pollWTDOG() {
    //    STA A,WTDOG
    // TODO: Not implemented
}

/**
 * EXPLOSION SOUND  D6-D7=PITCH,D2-D5=VOLUME
 * @param value
 */
void io_set_EXPSND(uint8_t value) {
    // TODO: Not implemented
}

/**
 * THUMP SOUND D4=1 FOR ON, D0-D3=FREQUENCY
 * @param value
 */
void io_set_THUMP(uint8_t value) {
    // TODO: Not implemented
}

/**
 * SAUCER THRUST SOUND,D7=1 FOR ON
 * @param value
 */
void io_set_SCRSND(uint8_t value) {
    // TODO: Not implemented
}

/**
 * SAUCER FIRE SOUND, D7=1 FOR ON
 * @param value
 */
void io_set_SCRFIR(uint8_t value) {
    // TODO: Not implemented
}

/**
 * SHIP THRUST SOUND, D7=1 FOR ON
 * @param value
 */
void io_set_SHPSND(uint8_t value) {
    // TODO: Not implemented
}

/**
 * SHIP FIRE SOUND,D7=1 FOR ON
 * @param value
 */
void io_set_SHPFIR(uint8_t value) {
    // TODO: Not implemented
}

/**
 * TONE FOR EXTRA LIFE, D7=1 FOR ON
 * @param value 
 */
void io_set_TONE(uint8_t value) {
    // TODO: Not implemented
}
