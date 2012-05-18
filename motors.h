#ifndef MOTORS_H
#define MOTORS_H

#include "timer2.h"

void motors_setup();
inline u16 motors_max_min(s32 value);

#define MOTORS_MAX 3990
#define MOTORS_MIN 2000

#define motors_set1(v) PWM1 = motors_max_min(v);
#define motors_set2(v) PWM2 = motors_max_min(v);
#define motors_set3(v) PWM3 = motors_max_min(v);
#define motors_set4(v) PWM4 = motors_max_min(v);

#endif
