#ifndef SYSTICK_H
#define SYSTICK_H

#include <libopencm3/cm3/common.h>

extern u32 systick_counter;
extern volatile u8 systick_flag;

void systick_setup(void);
void systick_handler(void);

#endif
