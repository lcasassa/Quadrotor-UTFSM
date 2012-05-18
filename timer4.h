#ifndef TIMER4_H
#define TIMER4_H

#include <libopencm3/stm32/timer.h>

void timer4_setup();

//#define PWM1 TIM4_CCR1
//#define PWM7 TIM4_CCR2
#define PWM7 TIM4_CCR3
#define PWM8 TIM4_CCR4



#endif
