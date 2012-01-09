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

	/* Update interrupt enable */
	TIM_DIER(TIM2) |= TIM_DIER_UIE;

	/* Start timer */
	TIM_CR1(TIM2) |= TIM_CR1_CEN;

}

void tim2_isr() { // 1ms
        timer_clear_flag(TIM2, TIM_SR_UIF);
	timer2_counter++;
	timer2_flag=1;
/*
	static u32 temp32 = 0;
	int altura, giro_x=0, giro_y, giro_z;

	temp32++;
//	printf("t");

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
*/
/*
	if (temp32 == 1000) { // 10 ms
//		printf("A2: %5d %5d %5d\r\n", (int)acelerometer[0], (int)acelerometer[1], (int)acelerometer[2]);
//		printf("A: (%8.3f, %8.3f, %8.3f) G: (%8.3f, %8.3f, %8.3f)\r\n", (double)((float)acelerometer[0]*0.0078)*9.81, (double)((float)acelerometer[1]*0.0078)*9.81, (((float)acelerometer[2])*0.0078)*9.81, (float)gyroscope[0]/14.375, (float)gyroscope[1]/14.375, (float)gyroscope[2]/14.375);
//		printf("%d %d %d %d %d %d\r\n", acelerometer[0], acelerometer[1], acelerometer[2], gyroscope[0], gyroscope[1], gyroscope[2]);
		//printf("%4.2f %4.2f => ", 180*atan2(acelerometer[0],acelerometer[2])/M_PI, ((double)gyroscope[1]/14.375) - k[0].x_hat[1]);
	//	printf("%4.2f %4.2f (%4.2f - %4.2f) => ", 180*atan2(acelerometer[1],acelerometer[2])/M_PI, ((double)gyroscope[0]/14.375) - 180*k[0].x_hat[1]/M_PI, ((double)gyroscope[0]/14.375), 180*k[0].x_hat[1]/M_PI);
		printf("%5d %5d %5d %5d %5d %5d %5d %5d\r\n", joystick[0], joystick[1], joystick[2], joystick[3], joystick[4], joystick[5], joystick[6], joystick[7]);
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
  */
}

