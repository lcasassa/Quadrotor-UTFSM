#ifndef TIMER1_H
#define TIMER1_H

#include <libopencm3/stm32/timer.h>

void timer1_setup();

#define READ_TIMER_1 TIM_CNT(TIM1)
#define TIMER1_PERIOD 60000

#endif
