#pragma once

#define ASTEROIDS_VG_A35131_1C_H

#include "commons.h"

void VGSABS(uint8_t A_x, uint8_t X_y);

void VGLABS(uint16_t x, uint16_t y);

void VGHALT();

void VGWAIT(uint8_t A_timer);

void VGJMPL(uint16_t vg_jsr_destination);

void VGJSRL(uint16_t vg_jsr_destination);

bool ok_VGHEX(uint8_t A_digit);

bool VGHEXZ(uint8_t A_digit, bool C_zero_suppression);

void VGADD(uint8_t Y_delta);

void VGDOT(uint8_t A_timer, uint8_t X_intensity);

#undef ASTEROIDS_VG_A35131_1C_H
