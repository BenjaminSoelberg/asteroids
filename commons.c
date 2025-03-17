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
