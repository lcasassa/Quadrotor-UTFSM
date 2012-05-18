#include <libopencm3/stm32/f1/flash.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/f1/scb.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/nvic.h>
#include <libopencm3/stm32/systick.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include "ringbuffer.h"
#include "main.h"
#include "usart.h"
#include "i2c1.h"
#include "i2c2.h"
#include "ADXL345.h"
#include "ITG3200.h"
#include "timer3.h"
#include "timer4.h"
#include "motors.h"
#include "timer1.h"
#include "timer2.h"
#include "exti_.h"
#include "joystick.h"
#include "nvic_.h"
#include "alfabeta.h"
#include "flash_.h"
#include "pid.h"
#include "parser.h"
#include "estabilizador.h"
#include "gyro_futaba.h"
#include "systick.h"
#include "BMP085.h"

#define printf_blocking(...) sprintf(__VA_ARGS__); for(i=0; s[i]!=0; i++) usart_send_blocking(USART1, s[i])
#define nop_delay() for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); }

#define LED_ON()     gpio_set   (GPIOC, GPIO12)
#define LED_OFF()    gpio_clear (GPIOC, GPIO12)
#define LED_TOGGLE() gpio_toggle(GPIOC, GPIO12)

int output = 0;

int main(void)
{
	long long int i;

	clock_setup();

	gpio_setup();
	LED_ON();

	usart_setup();

	printf("\r\n\r\n");
	printf("************************\r\n");
	printf("*   Quadrotor  UTFSM   *\r\n");
	printf("* %s %s *\r\n", __DATE__, __TIME__);
//	printf("* Mar 12 2012 10:27:34 *");
	printf("************************\r\n");

	pid_setup(&pid[0]);
	pid_setup(&pid[1]);
	pid_setup(&pid[2]);
	pid_setup(&pid[3]);

#ifndef I2C_OFF
	// Acelerometer and Gyroscope
	i2c2_setup();
	ADXL345_setup();
	ITG3200_setup();
#endif

	// RC Radio: 8 input PWM	
	timer1_setup();
	joystick_setup();
	exti_setup();

	alfabeta_setup();
	estabilizador_setup();

	if(flash_load()) printf("Restored from Flash\r\n");
	else printf("Not restored from Flash\r\n");

	// Enable interrupts
	systick_setup();
	nvic_setup();

	gpio_toggle(GPIOC, GPIO12);
	LED_OFF();


	nop_delay();

	// 4 output PWM (ESC) + 2 output PWM(camera servos) + 1 output PWM (Gyro Futaba)
	// 1 input PWM (Gyro Futaba)
	timer4_setup();
	motors_setup();
	timer2_setup();
	timer3_setup();

	i2c1_setup();
	BMP085_setup();

	while(!gyroscope_is_calibration_checked);

	while (1) {
		static u32 temp32 = 0;

		if(systick_flag) {
			systick_flag = 0;

			temp32++; // 2ms


			if ((temp32%(100/2)) == 0) { // 30 ms
				switch(output) {
				case 0:
BMP085_getValues();
if(temperature<0 || pressure<0 ||altitude<0) break;
printf("%d.%dC %dPa %d.%dm\r\n", (int)(temperature/10), (int)temperature%10, (int)(pressure), (int)(altitude), (int)(altitude*1000)%1000);
					break;
				case 1:
					printf("%d %d %d %d %d %d %d %d\r\n", (int)(gyroscope[0]*100), (int)(gyroscope[1]*100), (int)(gyroscope[2]*100), (int)(angle[0]*100), (int)(angle[1]*100), (int)(angle[2]*100), (int)(180.0*atan2((double)-acelerometer[0],(double)acelerometer[2])/M_PI*100), (int)(-180.0*atan2((double)-acelerometer[1],(double)acelerometer[2])/M_PI*100));
					break;
				case 2:
					printf("%d %d %d %d %d %d\r\n", (int)(gyroscope[0]*100), (int)(joystick[0]*100), (int)(gyroscope[1]*100), (int)(joystick[1]*100), (int)(gyroscope[2]*100), (int)(joystick[3]*100));
					break;
				case 3:
					printf("%d %d %d %d\r\n", (int)(angle[0]*100), (int)(joystick[0]*10), (int)(angle[1]*100), (int)(joystick[1]*10));
					break;
				case 4:
//					printf("%d %d %d %d %d %d %d %d\t", (int)joystick_update_count[0], (int)joystick_update_count[1], (int)joystick_update_count[2], (int)joystick_update_count[3], (int)joystick_update_count[4], (int)joystick_update_count[5], (int)joystick_update_count[6], (int)joystick_update_count[7]);
					printf("%d %d %d %d %d %d %d %d\r\n", (int)joystick[0], (int)joystick[1], (int)joystick[2], (int)joystick[3], (int)joystick[4], (int)joystick[5], (int)joystick[6], (int)joystick[7]);
				}
//				printf("%d\n", (int)(100.0*180.0*atan2((double)-acelerometer[0],(double)acelerometer[2])/M_PI)); //X
//				printf("%d\n", (int)gyroscope[0]); //X
//				printf("%d %d %d %d 0\n", (int)(angle[0]*100), (int)(angle[1]*100), (int)(angle_gyro[0]*100), (int)(angle_gyro[1]*100));
//				printf("%d %d %d 0 %d\r\n", joystick[0], joystick[1], joystick[3], joystick[2]);

#if 0
				printf("%5d %5d %5d %5d %5d %5d %5d %5d\r\n", joystick[0], joystick[1], joystick[2], joystick[3], joystick[4], joystick[5], joystick[6], joystick[7]);
				printf("%d %d %d %d %d %d %d %d\r\n", joystick_update_count[0], joystick_update_count[1], joystick_update_count[2], joystick_update_count[3], joystick_update_count[4], joystick_update_count[5], joystick_update_count[6], joystick_update_count[7]);
#endif
				temp32 = 0;
			}


		}


		parser_check();
	}

	return 0;
}



void hard_fault_handler(void) {
	//    printf("Error: hard_fault_handler() !\r\n");
	char s[100];
	int i,j;

	for(j=0; j<10; j++) {
		gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
		for (i = 0; i < 800000; i++)    /* Wait a bit. */
			__asm__("nop");
	}

	printf_blocking(s, "[Hard fault handler]\r\n");

	if(SCB_HFSR_VECTTBL & SCB_HFSR) {
	  printf_blocking(s, "SCB_HFSR_VECTTBL: Bus Fault on vector table read.\r\n");
	}

	/*
	SCB_CFSR_DIVBYZERO   
	SCB_CFSR_UNALIGNED
	SCB_CFSR_NOCP
	SCB_CFSR_INVPC
	SCB_CFSR_INVSTATE
	SCB_CFSR_UNDEFINSTR
	*/
	//printf_blocking(s, "SCB_CFSR: 0x%lx\r\n", SCB_CFSR);
	//printf_blocking(s, "SCB_BFAR: 0x%08lx\r\n", SCB_BFAR);

	if(SCB_CFSR_BFARVALID & SCB_CFSR) {
	  printf_blocking(s, "SCB_CFSR_BFARVALID: BFAR holds a valid fault address.\r\n");
	}
	if(SCB_CFSR_STKERR & SCB_CFSR) {
	  printf_blocking(s, "STKERR\r\n");
	}
	if(SCB_CFSR_UNSTKERR & SCB_CFSR) {
	  printf_blocking(s, "UNSTKERR\r\n");
	}
	if(SCB_CFSR_IMPRECISERR & SCB_CFSR) {
	  printf_blocking(s, "IMPRECISERR\r\n");
	}
	if(SCB_CFSR_PRECISERR & SCB_CFSR) {
	  printf_blocking(s, "SCB_CFSR_PRECISERR: A data bus error has occurred,");
	  printf_blocking(s, " and the PC value stacked for the exception return ");
	  printf_blocking(s, "points to the instruction that caused the fautl.\r\n");
	}
	if(SCB_CFSR_IBUSERR & SCB_CFSR) {
	  printf_blocking(s, "IBUSERR\r\n");
	}
	if(SCB_CFSR_MMARVALID & SCB_CFSR) {
	  printf_blocking(s, "MMARVALID\r\n");
	}
	if(SCB_CFSR_MSTKERR & SCB_CFSR) {
	  printf_blocking(s, "MSTKERR\r\n");
	}
	if(SCB_CFSR_MUNSTKERR & SCB_CFSR) {
	  printf_blocking(s, "SCB_CFSR_MUNSTKERR: Unstacking for an exception return has caused");
	  printf_blocking(s, " one or more access violations\r\n");
	}
	if(SCB_CFSR_DACCVIOL & SCB_CFSR) {
	  printf_blocking(s, "SCB_CFSR_DACCVIOL: The processor attempted a load or store at a");
	  printf_blocking(s, " location that does not permit the operation.\r\n");
	}

	if(SCB_CFSR_IACCVIOL & SCB_CFSR) {
	  printf_blocking(s, "SCB_CFSR_IACCVIOL: The processor attempted an instruction fetch from");
	  printf_blocking(s, " a location that does not permit execution.\r\n");
	}



//	while(1);
}

void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable GPIOA clock (for LED GPIOs). */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);

	/* Enable clocks for GPIO port A (for GPIO_USART1_TX) and USART1. */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN |
				    RCC_APB2ENR_AFIOEN | RCC_APB2ENR_USART1EN);
}

void gpio_setup(void)
{
	gpio_set(GPIOC, GPIO12);

	/* Setup GPIO6 and 7 (in GPIO port A) for LED use. */
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
}

