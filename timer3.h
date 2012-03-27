#ifndef TIMER3_H
#define TIMER3_H

#include <libopencm3/stm32/timer.h>

void timer3_setup();

//#define PWM1 TIM4_CCR1
//#define PWM7 TIM3_CCR2
#define PWM5 TIM3_CCR3
#define PWM6 TIM3_CCR4


#endif
