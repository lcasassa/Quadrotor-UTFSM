#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include "timer3.h"
#include "gyro_futaba.h"

void timer3_setup(void)
{


	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM3EN);

	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);
//				    RCC_APB2ENR_IOPAEN |
//				    RCC_APB2ENR_AFIOEN);

	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		      GPIO_TIM3_CH3 | GPIO_TIM3_CH4);

//	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
//		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
//		      GPIO_TIM3_CH2);

        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_TIM3_CH1);

	// Counter disable
        TIM_CR1(TIM3) &= ~TIM_CR1_CEN;

	/* Clock division and mode */
	TIM3_CR1 = TIM_CR1_CKD_CK_INT | TIM_CR1_CMS_EDGE;
	/* Prescaler */
	TIM3_PSC = 36; // 72MHz/36 = 2MHz => 2000clk -> 1ms
	/* Period */
	TIM3_ARR = 40000; // periodo del pwm 20ms

        // Set timer start value
        TIM_CNT(TIM3) = 1;

//	TIM3_EGR = TIM_EGR_UG;

	/* ---- */
	/* Output compare 1 mode and preload */
//	TIM3_CCMR1 |= TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;

	/* Polarity and state */
	// TIM3_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
//	TIM3_CCER |= TIM_CCER_CC1E;

	/* Capture compare value */
//	TIM3_CCR1 = 2000;

	// Capture PWM
	TIM3_CCMR1 = (0x01 << 0); //TIM_CCMR1_CC1S_IN_TI1;
	TIM3_CCMR1 |= TIM_CCMR1_IC1F_CK_INT_N_8;
	TIM3_CCMR1 |= TIM_CCMR1_IC1PSC_OFF;
	//TIM3_CCMR1 = TIM_CCMR1_CC1S_IN_TI1 | TIM_CCMR1_IC1F_CK_INT_N_8 | TIM_CCMR1_IC1PSC_OFF;

	TIM3_CCER &= ~TIM_CCER_CC1P; // canto subida

	TIM3_CCMR1 |= TIM_CCMR1_CC2S_IN_TI1;
	TIM3_CCMR1 |= TIM_CCMR1_IC2F_CK_INT_N_8;
	TIM3_CCMR1 |= TIM_CCMR1_IC2PSC_OFF;

	TIM3_CCER |= TIM_CCER_CC2P; // canto bajada

	TIM3_SMCR = TIM_SMCR_TS_IT1FP1;
	TIM3_SMCR |= TIM_SMCR_SMS_RM;


	TIM3_CCER |= TIM_CCER_CC1E;
	TIM3_CCER |= TIM_CCER_CC2E;

	TIM3_DIER = TIM_DIER_CC1IE;

/*
	// ---- 
	// Output compare 2 mode and preload 
	TIM3_CCMR1 |= TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;

	// Polarity and state 
	//TIM3_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM3_CCER |= TIM_CCER_CC2E;

	// Capture compare value 
	TIM3_CCR2 = 3000;
*/
	// ---- 
	// Output compare 3 mode and preload 
	TIM3_CCMR2 |= TIM_CCMR2_OC3M_PWM1 | TIM_CCMR2_OC3PE;

	// Polarity and state 
	//TIM3_CCER = TIM_CCER_CC3P | TIM_CCER_CC3E;
	TIM3_CCER |= TIM_CCER_CC3E;

	// Capture compare value 
	TIM3_CCR3 = 0;

	// ---- 
	// Output compare 4 mode and preload 
	TIM3_CCMR2 |= TIM_CCMR2_OC4M_PWM1 | TIM_CCMR2_OC4PE;

	// Polarity and state 
	//TIM3_CCER = TIM_CCER_CC4P | TIM_CCER_CC4E;
	TIM3_CCER |= TIM_CCER_CC4E;

	// Capture compare value 
	TIM3_CCR4 = 0;

	// ---- */
	// ARR reload enable 
//	TIM3_CR1 |= TIM_CR1_ARPE;

	// Update interrupt enable 
//	TIM_DIER(TIM3) |= TIM_DIER_UIE;
	TIM_DIER(TIM3) &= ~TIM_DIER_UIE;

	// Counter enable 
	TIM3_CR1 |= TIM_CR1_CEN;

}

void tim3_isr(void) {
	volatile u32 tmp;
//	printf("%u \t %u\r\n", (unsigned int)TIM3_CCR1, (unsigned int)TIM3_CCR2);
	gyro_futaba_timer_capture(TIM3_CCR2);
	// clear flag
	tmp = TIM3_CCR1;
}

