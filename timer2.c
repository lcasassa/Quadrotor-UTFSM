#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include "timer2.h"

void timer2_setup(void)
{

	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN);

	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN);

	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_TIM2_CH1_ETR | GPIO_TIM2_CH2 | GPIO_TIM2_CH3 | GPIO_TIM2_CH4);

	// Counter disable
        TIM_CR1(TIM2) &= ~TIM_CR1_CEN;

	/* Clock division and mode */
	TIM2_CR1 = TIM_CR1_CKD_CK_INT | TIM_CR1_CMS_EDGE;
	/* Prescaler */
	TIM2_PSC = 36; // 72MHz/36 = 2MHz => 2000clk -> 1ms
	/* Period */
	TIM2_ARR = 4000; // periodo del pwm 500 Hz -> 2000*1000/500 = 4000

        // Set timer start value
        TIM_CNT(TIM2) = 1;

	TIM2_EGR = TIM_EGR_UG;



	// ----
	// Output compare 1 mode and preload
	TIM2_CCMR1 |= TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;

	// Polarity and state
	// TIM4_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM2_CCER |= TIM_CCER_CC1E;

	// Capture compare value
	TIM2_CCR1 = 0;

	// ----
	// Output compare 2 mode and preload
	TIM2_CCMR1 |= TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;

	// Polarity and state
	// TIM2_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM2_CCER |= TIM_CCER_CC2E;

	// Capture compare value
	TIM2_CCR2 = 0;

	// ----
	// Output compare 3 mode and preload 
	TIM2_CCMR2 |= TIM_CCMR2_OC3M_PWM1 | TIM_CCMR2_OC3PE;

	// Polarity and state 
	//TIM3_CCER = TIM_CCER_CC3P | TIM_CCER_CC3E;
	TIM2_CCER |= TIM_CCER_CC3E;

	// Capture compare value 
	TIM2_CCR3 = 0;

	// ----
	// Output compare 4 mode and preload 
	TIM2_CCMR2 |= TIM_CCMR2_OC4M_PWM1 | TIM_CCMR2_OC4PE;

	// Polarity and state 
	//TIM3_CCER = TIM_CCER_CC4P | TIM_CCER_CC4E;
	TIM2_CCER |= TIM_CCER_CC4E;

	// Capture compare value 
	TIM2_CCR4 = 0;

	// ----
	// ARR reload enable 
	TIM2_CR1 |= TIM_CR1_ARPE;

	// Update interrupt enable 
//	TIM_DIER(TIM3) |= TIM_DIER_UIE;
//	TIM_DIER(TIM2) &= ~TIM_DIER_UIE;

	// Counter enable 
	TIM2_CR1 |= TIM_CR1_CEN;

}

