#ifndef TIMER2_H
#define TIMER2_H

#include <libopencm3/stm32/timer.h>

void timer2_setup();

#define PWM1 TIM2_CCR1
#define PWM2 TIM2_CCR2
#define PWM3 TIM2_CCR3
#define PWM4 TIM2_CCR4

#endif
