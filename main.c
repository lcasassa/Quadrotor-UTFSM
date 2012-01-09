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
#include "main.h"
#include "usart.h"
#include "i2c.h"
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
#include "kalman.h"
#include "flash_.h"
#include "pid.h"


int main(void)
{
	long long int i;
	clock_setup();
	gpio_setup();
	usart_setup();

	// Delay for the xBee link to be ready
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); }
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); }
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); }
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); }
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); }
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */

	printf("\r\n\r\n");
	printf("*********************\r\n");
	printf("* ADXL345 & ITG3200 *\r\n");
	printf("*********************\r\n");

	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); }
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */

	pid_setup(&pid[0]);
	pid_setup(&pid[1]);

	kalman_setup();

	// ESC: 4 output PWM
	timer4_setup();
	motors_setup();

	// Servo: 2 output PWM
	timer3_setup();

#ifndef I2C_OFF
	// Acelerometer and Gyroscope
	i2c_setup();
	ADXL345_setup();
	ITG3200_setup();
#endif

	// RC Radio: 8 input PWM	
	timer1_setup();
	joystick_setup();
	exti_setup();

	flash_load();

	// Enable interrupts
	timer2_setup();
	//systick_setup();
	nvic_setup();

	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); }
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); }
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	while (1) {
		static u32 temp32 = 0;
		int altura=0, giro_x=0, giro_y=0, giro_z=0;
		static u32 timer_old=0;
		u32 timer;

		timer = timer2_counter;
		if(timer2_flag) {
			timer2_flag = 0;

			temp32++;

			if (temp32%2 == 0) { // 2 ms
		#ifndef I2C_OFF
				ADXL345_getValues();
				ITG3200_getValues();
		#endif
				KalmanUpdate(0);
				KalmanUpdate(1);//&kalman[0], 180*atan2(acelerometer[1],acelerometer[2])/M_PI, (double)gyroscope[0]/14.375);
			}

			if (temp32%5 == 0) { // 5 ms
				//int altura, gyro_x, gyro_y, gyro_z;
				altura = joystick[2];
				giro_x = (int)(pid_update(&pid[0], ((float)(joystick[0]-500))/30.0, k[0].angle*180/M_PI)*100.0);
				giro_y = (int)(pid_update(&pid[1], ((float)(joystick[1]-500))/30.0, k[1].angle*180/M_PI)*100.0);
				giro_z = joystick[3] - 500;

				PWM1 = ( altura - giro_z - giro_x );
				PWM2 = ( altura + giro_z + giro_y );
				PWM3 = ( altura - giro_z + giro_x );
				PWM4 = ( altura + giro_z - giro_y );

			}

			if (temp32 == 50) { // 10 ms
				printf("%d %d %d 0 0\r\n", (int)(100*((float)(joystick[0]-500))/30.0), (int)(100.0*k[0].angle), giro_x);

				PWM5 = ((k[1].angle/M_PI*3600 + k[0].angle/M_PI*3600) + 1000) + 1000;
				PWM6 = ((k[1].angle/M_PI*3600 - k[0].angle/M_PI*3600) + 1000) + 1000;
				temp32 = 0;
			}


			timer_old = timer;
		}
	}

	return 0;
}

#define printf_blocking(...) sprintf(__VA_ARGS__); for(i=0; s[i]!=0; i++) usart_send_blocking(USART1, s[i])


static inline uint32_t read_lr(void)
{
       register uint32_t ret;
       asm volatile ("mrs %0, primask" : "=r" (ret));
       return ret;
}

void hard_fault_handler(void) {
	//    printf("Error: hard_fault_handler() !\r\n");
	register int R0 __asm__ ("r0");
	register int R1 __asm__ ("r1");
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
	  printf_blocking(s, ", and the PC value stacked for the exception return ");
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

	printf_blocking(s, "SCB_CFSR: 0x%lx\r\n", SCB_CFSR);
	printf_blocking(s, "SCB_BFAR: 0x%08lx\r\n", SCB_BFAR);

	asm( "MRS     R0,PSP");
	asm( "LDR     R1,[R0,#24]");
	sprintf(s, "R0(PSP) = %x\r\nR1(PC) = %x\r\n", R0, R1); for(i=0; s[i]!=0; i++) usart_send_blocking(USART1, s[i]);

	while(1);
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

