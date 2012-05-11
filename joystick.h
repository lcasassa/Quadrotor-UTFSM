#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <libopencm3/cm3/common.h>

extern s16 joystick[8];
extern s16 joystick_update_count[8];
extern s16 joystick_offset[8];

void joystick_setup();
void joystick_exti(u8 n, u16 time, u8 edge);

#endif
