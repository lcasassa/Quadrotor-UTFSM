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

	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); } // delay esperando a que serial se comunique
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); } // delay esperando a que serial se comunique
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); } // delay esperando a que serial se comunique
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); } // delay esperando a que serial se comunique
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); } // delay esperando a que serial se comunique
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	printf("\r\n\r\n");
	printf("*********************\r\n");
	printf("* ADXL345 & ITG3200 *\r\n");
	printf("*********************\r\n");
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); } // delay esperando a que serial se comunique
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); } // delay esperando a que serial se comunique
	gpio_toggle(GPIOC, GPIO12);     /* LED on/off */
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); } // delay esperando a que serial se comunique
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

	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); } // delay esperando a que serial se comunique
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); } // delay esperando a que serial se comunique
	for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); } // delay esperando a que serial se comunique
	while (1) {
		static u32 temp32 = 0;
		int altura=0, giro_x=0, giro_y=0, giro_z=0;
		static u32 timer_old=0;
		static u32 timer_count=0;
//		static u32 timer_count2=0;
		u32 timer;
//		u16 ejecuto=0;
//		u32 timer_raw;
		//__asm__("nop");
//		printf("%5d %5d %5d %5d %5d %5d %5d %5d\r\n", joystick[0], joystick[1], joystick[2], joystick[3], joystick[4], joystick[5], joystick[6], joystick[7]);
		//for(i=0; i<10000000; i++) { __asm__("nop");  __asm__("nop");  __asm__("nop"); __asm__("nop"); } // delay esperando a que serial se comunique

		timer = timer2_counter;
		if(timer2_flag) {
			timer2_flag = 0;
//			if((timer_count++%1024) == 0)
//				printf("H dt:%5ld joy[0]%5d\r\n", timer - timer_old, joystick[0]);




	temp32++;
//	printf("t");

	if (temp32%2 == 0) { // 2 ms
//ejecuto |= 1;
#ifndef I2C_OFF
		ADXL345_getValues();
		ITG3200_getValues();
#endif
		KalmanUpdate(0);
		KalmanUpdate(1);//&kalman[0], 180*atan2(acelerometer[1],acelerometer[2])/M_PI, (double)gyroscope[0]/14.375);
	}

	if (temp32%5 == 0) { // 5 ms
//ejecuto |= 2;
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
//ejecuto |= 4;
//		printf("A2: %5d %5d %5d\r\n", (int)acelerometer[0], (int)acelerometer[1], (int)acelerometer[2]);
//		printf("A: (%8.3f, %8.3f, %8.3f) G: (%8.3f, %8.3f, %8.3f)\r\n", (double)((float)acelerometer[0]*0.0078)*9.81, (double)((float)acelerometer[1]*0.0078)*9.81, (((float)acelerometer[2])*0.0078)*9.81, (float)gyroscope[0]/14.375, (float)gyroscope[1]/14.375, (float)gyroscope[2]/14.375);
//		printf("%d %d %d %d %d %d\r\n", acelerometer[0], acelerometer[1], acelerometer[2], gyroscope[0], gyroscope[1], gyroscope[2]);
		//printf("%4.2f %4.2f => ", 180*atan2(acelerometer[0],acelerometer[2])/M_PI, ((double)gyroscope[1]/14.375) - k[0].x_hat[1]);
	//	printf("%4.2f %4.2f (%4.2f - %4.2f) => ", 180*atan2(acelerometer[1],acelerometer[2])/M_PI, ((double)gyroscope[0]/14.375) - 180*k[0].x_hat[1]/M_PI, ((double)gyroscope[0]/14.375), 180*k[0].x_hat[1]/M_PI);
		//printf("dt:%5ld ", timer - timer_old);

//		printf("%5d %5d %5d %5d %5d %5d %5d %5d\r\n", joystick[0], joystick[1], joystick[2], joystick[3], joystick[4], joystick[5], joystick[6], joystick[7]);
//		printf("%5d %5d %5d %5d %5d %5d %5d %5d \t2\r\n", joystick2[0], joystick2[1], joystick2[2], joystick2[3], joystick2[4], joystick2[5], joystick2[6], joystick2[7]);

		//printf("%5d %5.2f %5.2f\r\n", giro_x, ((float)(joystick[0]-500))/30.0, k[0].angle);
		//printf("%4.2f\r\n", k[0].angle*180/M_PI);
//		printf("%d %d ", (int)(180*atan2((double)acelerometer[1],(double)acelerometer[2])/M_PI*100), (int)(k[0].angle*180/M_PI*100));
//		printf("%d %d %d\r\n", (int)(180*atan2((double)-acelerometer[0],(double)acelerometer[2])/M_PI*100), (int)(k[1].angle*180/M_PI*100), 0);
		//printf("%f\r\n", pid[0].P);
//		printf("%d\r\n", joystick[0]);

		printf("%d %d %d 0 0\r\n", (int)(100*((float)(joystick[0]-500))/30.0), (int)(100.0*k[0].angle), giro_x);

		PWM5 = ((k[1].angle/M_PI*3600 + k[0].angle/M_PI*3600) + 1000) + 1000;
		PWM6 = ((k[1].angle/M_PI*3600 - k[0].angle/M_PI*3600) + 1000) + 1000;
		temp32 = 0;
	}



//if(timer-timer_old > 1) printf("dt:%5ld ejecuto: %d %d %d\r\n", timer-timer_old, ejecuto&1, ejecuto&2, ejecuto&4);

			timer_old = timer;
		}
	/*
		timer = READ_TIMER_1;
		timer_raw = timer;
		if(timer_old >= TIMER1_PERIOD) timer += TIMER1_PERIOD;
		else {
			if(timer < (timer_old-2000)) {
				
			}
		}
		if(timer >= timer_old) { // 1ms
		//	if((timer_count++%1024) == 0)
		//		printf("H dt:%5ld joy[0]%5d\r\n", timer - timer_old, joystick[0]);
			if((timer_count++%1024) == 0)
			printf("s\r\n");
			timer_old = (timer_raw + 2000);
		} else {
			if((timer_count2++%1024) == 0)
			printf("n");
		}
*/
	}

	return 0;
}

#define printf_blocking(...) sprintf(__VA_ARGS__); for(i=0; s[i]!=0; i++) usart_send_blocking(USART1, s[i])
register int R0 __asm__ ("r0");
register int R1 __asm__ ("r1");


static inline uint32_t read_lr(void)
{
       register uint32_t ret;
       asm volatile ("mrs %0, primask" : "=r" (ret));
       return ret;
}

void nmi_handler(void) {
  char s[50];
  int i;

  printf_blocking(s, "[NMI fault handler]\n");
}

void mem_manage_handler(void) {
  char s[50];
  int i;

  printf_blocking(s, "[MEM fault handler]\n");
}

void bus_fault_handler(void) {
  char s[50];
  int i;

  printf_blocking(s, "[BUS fault handler]\n");
}

void usage_fault_handler(void) {
  char s[50];
  int i;

  printf_blocking(s, "[USAGE fault handler]\n");
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
sprintf(s, "R0(PSP) = %lx\r\nR1(PC) = %lx\r\n", R0, R1); for(i=0; s[i]!=0; i++) usart_send_blocking(USART1, s[i]);
//r0 = R0;
//printf_blocking(s, "R0(PSP) = %x\r\n", r0);
//sprintf(s, "R1(PC) = %x\r\n", R1); for(i=0; s[i]!=0; i++) usart_send_blocking(USART1, s[i]);
//r1 = R1;
//printf_blocking(s, "R1(PC) = %x\r\n", r1);

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

/*

void systick_setup(void)
{
	// 72MHz / 8 => 9000000 counts per second.
	systick_set_clocksource(STK_CTRL_CLKSOURCE_AHB_DIV8);

	// 9000000/9000 = 1000 overflows per second - every 1ms one interrupt
	systick_set_reload(9000);

	systick_interrupt_enable();

	// Start counting.
	systick_counter_enable();
}

void sys_tick_handler(void)
{
	static u32 temp32 = 0;
	int altura, giro_x=0, giro_y, giro_z;

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
		giro_x = (int)pid_update(&pid[0], ((float)(joystick[0]-500))/30.0, k[0].angle);
		giro_y = (int)pid_update(&pid[1], ((float)(joystick[1]-500))/30.0, k[1].angle);
		giro_z = joystick[3] - 500;

                PWM1 = ( altura - giro_z - giro_x );
                PWM2 = ( altura + giro_z + giro_y );
                PWM3 = ( altura - giro_z + giro_x );
                PWM4 = ( altura + giro_z - giro_y );

	}

	if (temp32 == 1000) { // 10 ms
//		printf("A2: %5d %5d %5d\r\n", (int)acelerometer[0], (int)acelerometer[1], (int)acelerometer[2]);
//		printf("A: (%8.3f, %8.3f, %8.3f) G: (%8.3f, %8.3f, %8.3f)\r\n", (double)((float)acelerometer[0]*0.0078)*9.81, (double)((float)acelerometer[1]*0.0078)*9.81, (((float)acelerometer[2])*0.0078)*9.81, (float)gyroscope[0]/14.375, (float)gyroscope[1]/14.375, (float)gyroscope[2]/14.375);
//		printf("%d %d %d %d %d %d\r\n", acelerometer[0], acelerometer[1], acelerometer[2], gyroscope[0], gyroscope[1], gyroscope[2]);
		//printf("%4.2f %4.2f => ", 180*atan2(acelerometer[0],acelerometer[2])/M_PI, ((double)gyroscope[1]/14.375) - k[0].x_hat[1]);
	//	printf("%4.2f %4.2f (%4.2f - %4.2f) => ", 180*atan2(acelerometer[1],acelerometer[2])/M_PI, ((double)gyroscope[0]/14.375) - 180*k[0].x_hat[1]/M_PI, ((double)gyroscope[0]/14.375), 180*k[0].x_hat[1]/M_PI);
		printf("%5d %5d %5d %5d %5d %5d %5d %5d %5lu\r\n", joystick[0], joystick[1], joystick[2], joystick[3], joystick[4], joystick[5], joystick[6], joystick[7], READ_TIMER_1);
		//printf("%5d %5d %5d %5d %5d %5d %5d %5d \t2\r\n", joystick2[0], joystick2[1], joystick2[2], joystick2[3], joystick2[4], joystick2[5], joystick2[6], joystick2[7]);
		//printf("%4.2f\r\n", k[0].angle*180/M_PI);
//		printf("%d %d ", (int)(180*atan2((double)acelerometer[1],(double)acelerometer[2])/M_PI*100), (int)(k[0].angle*180/M_PI*100));
//		printf("%d %d %d\r\n", (int)(180*atan2((double)-acelerometer[0],(double)acelerometer[2])/M_PI*100), (int)(k[1].angle*180/M_PI*100), 0);
		//printf("%f\r\n", pid[0].P);
//		printf("%d\r\n", joystick[0]);
		PWM5 = ((k[1].angle/M_PI*3600 + k[0].angle/M_PI*3600) + 1000) + 1000;
		PWM6 = ((k[1].angle/M_PI*3600 - k[0].angle/M_PI*3600) + 1000) + 1000;
		temp32 = 0;
	}
}
*/

