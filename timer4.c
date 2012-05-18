#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include "timer4.h"

void timer4_setup(void)
{
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM4EN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);

	//TIM4-CH3 -> PB8 -> EXT1-16
	//TIM4-CH4 -> PB9 -> EXT1-17
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_TIM4_CH3 | GPIO_TIM4_CH4);

	// Counter disable
        TIM_CR1(TIM4) &= ~TIM_CR1_CEN;

	// Clock division and mode
	TIM4_CR1 = TIM_CR1_CKD_CK_INT | TIM_CR1_CMS_EDGE;
	// Prescaler
	TIM4_PSC = 36; // 72MHz/36 = 2MHz => 2000clk -> 1ms
	// Period
	TIM4_ARR = 40000; // periodo del pwm 20 ms -> 2000*1000*0.020 = 4000

        // Set timer start value
        TIM_CNT(TIM4) = 1;

	TIM4_EGR = TIM_EGR_UG;


/*
	// ----
	// Output compare 1 mode and preload
	TIM4_CCMR1 |= TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;

	// Polarity and state
	// TIM4_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM4_CCER |= TIM_CCER_CC1E;

	// Capture compare value
	TIM4_CCR1 = 0;

	// ----
	// Output compare 2 mode and preload
	TIM4_CCMR1 |= TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;

	// Polarity and state
	// TIM4_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM4_CCER |= TIM_CCER_CC2E;

	// Capture compare value
	TIM4_CCR2 = 0;
*/
	// ----
	// Output compare 3 mode and preload
	TIM4_CCMR2 |= TIM_CCMR2_OC3M_PWM1 | TIM_CCMR2_OC3PE;

	// Polarity and state
	// TIM4_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM4_CCER |= TIM_CCER_CC3E;

	// Capture compare value
	TIM4_CCR3 = 0;

	// ----
	// Output compare 4 mode and preload
	TIM4_CCMR2 |= TIM_CCMR2_OC4M_PWM1 | TIM_CCMR2_OC4PE;

	// Polarity and state
	// TIM3_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM4_CCER |= TIM_CCER_CC4E;

	// Capture compare value
	TIM4_CCR4 = 0;

	// ARR reload enable
	TIM4_CR1 |= TIM_CR1_ARPE;

	// Update interrupt enable
//	TIM_DIER(TIM4) |= TIM_DIER_UIE;

	// Counter enable
	TIM4_CR1 |= TIM_CR1_CEN;

}

