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

/**
 * Will wait until the DVG has executed a HALT instruction.
 */
void wait_for_HALT() {
    //    10$:	LDA A,HALT
    //    BMI 10$			;WAIT FOR BEAM TO HALT
    // TODO: Remember to implement
}

void trigger_nmi() {
    //TODO: Remember to implement
    assert(false);
}

/**
 * I guess any value will start the vector generator
 */
void io_startGOADD() {
    // TODO Clear the HALT flag ?
    //    STA A,GOADD		;START VECTOR GENERATOR
    // TODO: Remember to implement
}

/**
 * I guess any value will poll the watchdog timer
 */
void io_pollWTDOG() {
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
