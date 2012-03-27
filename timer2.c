#include <libopencm3/stm32/f1/rcc.h>
#include "timer2.h"
#include <stdio.h> 
#include <math.h> 
#include "kalman.h"
#include "pid.h"
#include "timer3.h"
#include "timer4.h"
#include "joystick.h"
#include "ADXL345.h"
#include "ITG3200.h"

u32 timer2_counter=0;
volatile u8 timer2_flag=0;

void timer2_setup()
{
    // Timer 2 a 2MHz
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN);

	timer_reset(TIM2);

	/* Set timer start value */
	TIM_CNT(TIM2) = 1;

	/* Set timer prescaler. 72MHz/72 => 1MHz counts per second */
	timer_set_prescaler(TIM2, 72);

	/* End timer value. If this value is reached an interrupt is generated */
	timer_set_period(TIM2, TIMER2_PERIOD);

/*
	TIM2_CCMR1 |= TIM_CCMR1_CC1S_IN_TI1 | TIM_CCMR1_IC1F_CK_INT_N_8 | TIM_CCMR1_IC1PSC_OFF;
	TIM2_CCER |= TIM_CCER_CC1P | TIM_CCER_CC1E;
	timer_enable_irq(TIM2, TIM_DIER_CC1IE);
*/

	/* Update interrupt enable */
	TIM_DIER(TIM2) |= TIM_DIER_UIE;

	/* Start timer */
	TIM_CR1(TIM2) |= TIM_CR1_CEN;

}

void tim2_isr() { // 1ms
        timer_clear_flag(TIM2, TIM_SR_UIF);
	timer2_counter++;
	timer2_flag=1;
}

