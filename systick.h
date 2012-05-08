#ifndef SYSTICK_H
#define SYSTICK_H

#include <libopencm3/cm3/common.h>

extern u32 systick_counter;
extern volatile u8 systick_flag;
extern float angle[3];
extern float angle_gyro[3];


void systick_setup(void);
void sys_tick_handler(void);

#endif
