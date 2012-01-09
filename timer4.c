#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include "timer4.h"

void timer4_setup(void)
{
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM4EN);

	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO6 | GPIO7 | GPIO8 | GPIO9);

	/* Clock division and mode */
	TIM4_CR1 = TIM_CR1_CKD_CK_INT | TIM_CR1_CMS_EDGE;
	/* Prescaler */
	TIM4_PSC = 36; // 72MHz/36 = 2MHz => 2000clk -> 1ms
	/* Period */
	TIM4_ARR = 40000; // periodo del pwm 20ms
	TIM4_EGR = TIM_EGR_UG;

	/* ---- */
	/* Output compare 1 mode and preload */
	TIM4_CCMR1 |= TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;

	/* Polarity and state */
	// TIM4_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM4_CCER |= TIM_CCER_CC1E;

	/* Capture compare value */
	TIM4_CCR1 = 2000;

	/* ---- */
	/* Output compare 2 mode and preload */
	TIM4_CCMR1 |= TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;

	/* Polarity and state */
	// TIM4_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM4_CCER |= TIM_CCER_CC2E;

	/* Capture compare value */
	TIM4_CCR2 = 2000;

	/* ---- */
	/* Output compare 3 mode and preload */
	TIM4_CCMR2 |= TIM_CCMR2_OC3M_PWM1 | TIM_CCMR2_OC3PE;

	/* Polarity and state */
	// TIM4_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM4_CCER |= TIM_CCER_CC3E;

	/* Capture compare value */
	TIM4_CCR3 = 2000;

	/* ---- */
	/* Output compare 4 mode and preload */
	TIM4_CCMR2 |= TIM_CCMR2_OC4M_PWM1 | TIM_CCMR2_OC4PE;

	/* Polarity and state */
	// TIM3_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM4_CCER |= TIM_CCER_CC4E;

	/* Capture compare value */
	TIM4_CCR4 = 2000;

	/* ---- */
	/* ARR reload enable */
	TIM4_CR1 |= TIM_CR1_ARPE;

	/* Update interrupt enable */
//	TIM_DIER(TIM4) |= TIM_DIER_UIE;

	/* Counter enable */
	TIM4_CR1 |= TIM_CR1_CEN;

}
