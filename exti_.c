#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/nvic.h>
#include <libopencm3/stm32/exti.h>
#include "exti_.h"
#include "joystick.h"
#include "gyro_futaba.h"
#include "timer1.h"
#include <stdio.h>

/*
PORT - PIN     - Channel
PC6  - EXT2-20 - 4 GEAR
PC7  - EXT2-21 - 5 AUX1
PC8  - EXT2-22 - 6 AUX2
PC9  - EXT2-24 - 7 AUX3
PB15 - EXT2-19 - 3 RUDD
PB14 - EXT2-18 - 2 THRO
PB13 - EXT2-16 - 0 ELEV
PB12 - EXT2-17 - 1 AILE
*/

void exti_setup() {
	AFIO_EXTICR1 = 0x000;
	AFIO_EXTICR2 = 0x000;
	AFIO_EXTICR3 = 0x000;
	AFIO_EXTICR4 = 0x000;

	// Enable GPIOA clock.
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);

	// Enable AFIO clock.
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_AFIOEN);

	gpio_set_mode(GPIOC, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO6);
	exti_select_source(EXTI6, GPIOC);
	exti_set_trigger(EXTI6, EXTI_TRIGGER_RISING);
	exti_enable_request(EXTI6);

	gpio_set_mode(GPIOC, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO7);
	exti_select_source(EXTI7, GPIOC);
	exti_set_trigger(EXTI7, EXTI_TRIGGER_RISING);
	exti_enable_request(EXTI7);

	gpio_set_mode(GPIOC, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO8);
	exti_select_source(EXTI8, GPIOC);
	exti_set_trigger(EXTI8, EXTI_TRIGGER_RISING);
	exti_enable_request(EXTI8);

	gpio_set_mode(GPIOC, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO9);
	exti_select_source(EXTI9, GPIOC);
	exti_set_trigger(EXTI9, EXTI_TRIGGER_RISING); //EXTI_TRIGGER_BOTH);
	exti_enable_request(EXTI9);

	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO15);
	exti_select_source(EXTI15, GPIOB);
	exti_set_trigger(EXTI15, EXTI_TRIGGER_RISING);
	exti_enable_request(EXTI15);

	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO14);
	exti_select_source(EXTI14, GPIOB);
	exti_set_trigger(EXTI14, EXTI_TRIGGER_RISING);
	exti_enable_request(EXTI14);

	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO13);
	exti_select_source(EXTI13, GPIOB);
	exti_set_trigger(EXTI13, EXTI_TRIGGER_RISING);
	exti_enable_request(EXTI13);

	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO12);
	exti_select_source(EXTI12, GPIOB);
	exti_set_trigger(EXTI12, EXTI_TRIGGER_RISING); //EXTI_TRIGGER_BOTH);
	exti_enable_request(EXTI12);

//	exti_reset_request(EXTI6 | EXTI7 | EXTI8 | EXTI9 | EXTI12 | EXTI13 | EXTI14 | EXTI15);
}

void exti9_5_isr(void) {
	u16 EXTI_PR_ = EXTI_PR;
	exti_reset_request(EXTI_PR_ & (EXTI6|GPIO7|GPIO8|GPIO9));
	do {
		u16 time = READ_TIMER_1;
		if(EXTI_PR_ & GPIO6) {
			joystick_exti(4, time);
		}
		if(EXTI_PR_ & GPIO7) {
			joystick_exti(5, time);
		}

		if(EXTI_PR_ & GPIO8) {
			joystick_exti(6, time);
		}

		if(EXTI_PR_ & GPIO9) {
			joystick_exti(7, time);
		}
		EXTI_PR_ = EXTI_PR;
		exti_reset_request(EXTI_PR_ & (EXTI6|GPIO7|GPIO8|GPIO9));
	} while(EXTI_PR_ & (GPIO6|GPIO7|GPIO8|GPIO9));
}

void exti15_10_isr(void) {
	u16 EXTI_PR_ = EXTI_PR;
	exti_reset_request(EXTI_PR_ & (EXTI12|GPIO13|GPIO14|GPIO15));
	do {
		u16 time = READ_TIMER_1;
		if(EXTI_PR_&GPIO15) {
			joystick_exti(3, time);
		}
		if(EXTI_PR_&GPIO14) {
			joystick_exti(2, time);
		}
		if(EXTI_PR_&GPIO13) {
			joystick_exti(0, time);
		}
		if(EXTI_PR_&GPIO12) {
			joystick_exti(1, time);
		}
		EXTI_PR_ = EXTI_PR;
		exti_reset_request(EXTI_PR_ & (EXTI12|GPIO13|GPIO14|GPIO15));
	} while (EXTI_PR_&(GPIO12|GPIO13|GPIO14|GPIO15));
}

