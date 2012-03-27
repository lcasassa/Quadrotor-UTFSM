#ifndef TIMER2_H
#define TIMER2_H

#include <libopencm3/stm32/timer.h>

//#define I2C_OFF 1

extern u32 timer2_counter;
extern volatile u8 timer2_flag;

void timer2_setup();

#define READ_TIMER_2 TIM_CNT(TIM2)
//#define TIMER2_PERIOD 1000 // 1 ms
#define TIMER2_PERIOD 1000 // 1 ms

#endif
