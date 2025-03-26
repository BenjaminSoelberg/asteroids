#pragma once

#define ASTEROIDS_VG_A35131_1C_H

#include "commons.h"

void todo_VGSABS(uint8_t A_x, uint8_t X_y);

void VGHALT();

void VGWAIT(uint8_t A_timer);

void VGJMPL(uint16_t vg_jsr_destination);

void VGJSRL(uint16_t vg_jsr_destination);

void VGHEX(uint8_t A_digit);

#undef ASTEROIDS_VG_A35131_1C_H
