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
#include "parser.h"
#include "estabilizador.h"
#include "gyro_futaba.h"

#define printf_blocking(...) sprintf(__VA_ARGS__); for(i=0; s[i]!=0; i++) usart_send_blocking(USART1, s[i])
#define nop_delay() for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); }

int main(void)
{
	long long int i;
	int j;
	clock_setup();
	gpio_setup();
	usart_setup();

	// Delay for the xBee link to be ready
	nop_delay();
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	nop_delay();
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	nop_delay();
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	nop_delay();
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	nop_delay();
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */

	printf("\r\n\r\n");
	printf("*********************\r\n");
	printf("* ADXL345 & ITG3200 *\r\n");
	printf("*********************\r\n");

	nop_delay();
	gpio_toggle(GPIOC, GPIO12);     // LED on/off

	pid_setup(&pid[0]);
	pid_setup(&pid[1]);
	pid_setup(&pid[2]);
	pid_setup(&pid[3]);
	

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
	estabilizador_setup();

	// Enable interrupts
	timer2_setup();
	//systick_setup();
	nvic_setup();
/*
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); }
	gpio_toggle(GPIOC, GPIO12);     // LED on/off 
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); }
	gpio_toggle(GPIOC, GPIO12);     // LED on/off 
*/
	while (1) {
	//	static u16 count=0;
		static u32 temp32 = 0;
		static s32 gyroscope_sum[3] = {0,0,0};
		int altura=0, giro_x=0, giro_y=0, giro_z=0;
//		static u32 timer_old=0;
		u32 timer;
		s16 joystick_[8]={0,0,0,0,0,0,0,0};
		s16 gyro_futaba_=0;
//		u8 i=0;

		timer = timer2_counter;
		if(timer2_flag) {
			timer2_flag = 0;

			temp32++;

			if ((temp32%2) == 0) { // 2 ms
		#ifndef I2C_OFF
				ADXL345_getValues();
				ITG3200_getValues();
		#endif
				KalmanUpdate(0);
				KalmanUpdate(1);//&kalman[0], 180*atan2(acelerometer[1],acelerometer[2])/M_PI, (double)gyroscope[0]/14.375);
				for(j=0; j<3; j++) {
					gyroscope_sum[j] += gyroscope[j];
				}
			}

			if ((temp32%5) == 0) { // 5 ms
				s32 motor[4];
//				TIM4_CNT = 1;
		//		set_primask();
				for(i=0;i<8;i++) {
					joystick_[i] = joystick[i]*2;
				}
				PWM5 = joystick_[3] + 2000;
				gyro_futaba_ = gyro_futaba;
		//		reset_primask();
				//int altura, gyro_x, gyro_y, gyro_z;
				altura = joystick_[2]+2000;
				giro_x = (int)(pid_update(&pid[0], ((float)(joystick_[0]-1000))*2, gyroscope[0]));
				giro_y = (int)(pid_update(&pid[1], ((float)(joystick_[1]-1000))*2, gyroscope[1]));
				giro_z = gyro_futaba_ - 1000;
				giro_z /= 4;

//printf("a%d\r\n", giro_z);

				
				motor[0] = ( altura - giro_z + giro_x );
				motor[1] = ( altura + giro_z + giro_y );
				motor[2] = ( altura - giro_z - giro_x );
				motor[3] = ( altura + giro_z - giro_y );
	
				for(j=0; j < 4; j++) {
					if(motor[j]<000) motor[j]=000;
					if(motor[j]>4000) motor[j]=4000;
				}

//motor[0]=0;
//motor[1]=0;
//motor[2]=0;
//motor[3]=0;

				if(joystick_[4] > 200) {
					PWM1 = motor[1];
					PWM2 = motor[2];
					PWM3 = motor[0];
					PWM4 = motor[3];
				} else {
					PWM1 = 2000;
					PWM2 = 2000;
					PWM3 = 2000;
					PWM4 = 2000;
				}

				for(j=0; j<3; j++) {
					gyroscope_sum[j] = 0;
				}

			}

			if ((temp32%20) == 0) { // 10 ms
				estabilizador_loop(0,0);
				// gyro_futaba
				//PWM7 = joystick_[3] + 2000;
			}

			if ((temp32) >= 60) { // 10 ms
printf("a%4d %4d %4d %4d\r\n", altura, giro_x, giro_y, giro_z);
//				count += 10;
//				char s[20];
//				printf("%d %d %d 0 0\r\n", (int)(100*((float)(joystick_[1]-500))/30.0), (int)(100.0*k[1].angle*180/M_PI), giro_y);

//if(-50 < gyro_futaba_ && gyro_futaba < 2000)
//printf("a: %d %d %d %d\r\n", altura, giro_x, giro_y, giro_z);
			//	printf("%d %d %d %d %d %d %d %d\r\n", joystick_[0], joystick_[1], joystick_[2], joystick_[3], joystick_[4], joystick_[5], joystick_[6], joystick_[7]);
//				printf("%d %d %d %d %d %d %d %d\r\n", joystick2[0], joystick2[1], joystick2[2], joystick2[3], joystick2[4], joystick2[5], joystick2[6], joystick2[7]);
		//		printf_blocking(s, "0 0 0 0 0\r\n");

//				if(count>40000) count=0;
				//printf("%u %u %u\r\n", (unsigned int)PWM5, (unsigned int)PWM6, 0);
				//printf("%u %u %u %u\r\n", (unsigned int)PWM1/2, (unsigned int)PWM2/2, (unsigned int)PWM3/2, (unsigned int)PWM4/2);
				temp32 = 0;
			}


//			timer_old = timer;
		}
/*
		if(usart_printf_flag) {
			s32 data;
			usart_printf_flag = 0;
			while((data = ring_read_ch(&output_ring_2, NULL)) != -1) {
				printf("%c", (char)data);
				//putchar((char)data);
			}
		}
*/
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

