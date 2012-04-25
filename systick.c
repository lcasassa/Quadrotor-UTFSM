#include <stdio.h>
#include <math.h>
#include <libopencm3/stm32/systick.h>
#include <libopencm3/stm32/f1/gpio.h>
#include "systick.h"
#include "alfabeta.h"
#include "ADXL345.h"
#include "ITG3200.h"
#include "joystick.h"
#include "pid.h"
#include "motors.h"
#include "gyro_futaba.h"
#include "estabilizador.h"

u32 systick_counter = 0;
volatile u8 systick_flag = 0;
float angle[2] = {0.0,0.0};
float angle_gyro[2] = {0.0,0.0};

void systick_setup(void)
{
        /* 72MHz / 8 => 9000000 counts per second. */
        systick_set_clocksource(STK_CTRL_CLKSOURCE_AHB_DIV8);

        /* 9000000/(9000*2) = 500 overflows per second - every 2ms one interrupt */
        systick_set_reload(18000);

        systick_interrupt_enable();

        /* Start counting. */
        systick_counter_enable();
}
#define TEST_PIN2_ON gpio_set (GPIOC, GPIO1);
#define TEST_PIN2_OFF gpio_clear (GPIOC, GPIO1);

void sys_tick_handler(void)
{
TEST_PIN2_ON;
	static u16 count=0;
	s32 altura=0, giro_x=0, giro_y=0, giro_z=0;
	s32 motor[4];
	float omega_ref[2] = {0.0, 0.0};

        systick_counter++;
        systick_flag=1;

	ADXL345_getValues();
	ITG3200_getValues();

	if(!gyroscope_is_calibration_checked) {
		if(!gyroscope_is_calibrated) {
			ITG3200_calibrate();
		} else {
			ITG3200_check_calibration();
		}
TEST_PIN2_OFF;
		return;
	}
	angle_gyro[0] += gyroscope[0]*1.15/14.375;
	angle_gyro[1] += gyroscope[1]*1.15/14.375;

	angle[0] = alfabeta_loop(&alfabeta[0], 180.0*atan2((double)-acelerometer[0],(double)acelerometer[2])/M_PI, (float)gyroscope[1]*1.15/14.375);
	angle[1] = alfabeta_loop(&alfabeta[1], -180.0*atan2((double)-acelerometer[1],(double)acelerometer[2])/M_PI, (float)gyroscope[0]*1.15/14.375);


	count++;
	if ((count%(10/2)) == 0) { // 10 ms
		estabilizador_loop(0,0);
	}
	if ((count%(50/2)) == 0) { // 50 ms
		omega_ref[0] = pid_update(&pid[2], ((float)joystick[0])/30, angle[0]);
		omega_ref[1] = pid_update(&pid[3], ((float)joystick[1])/30, angle[1]);
//		omega_ref[0] *= -100;
//		omega_ref[1] *= -100;
		count = 0;
	}

	giro_z = gyro_futaba(joystick[3]);

	altura = joystick[2];
	if(pid[2].P == 0.0 && pid[3].P == 0.0 ) {
		giro_x = (int)(pid_update(&pid[0], ((float)joystick[0])*2, gyroscope[0]));
		giro_y = (int)(pid_update(&pid[1], ((float)joystick[1])*2, gyroscope[1]));
	} else {
		giro_x = (int)(pid_update(&pid[0], omega_ref[0], gyroscope[0]));
		giro_y = (int)(pid_update(&pid[1], omega_ref[1], gyroscope[1]));
	}

	motor[0] = ( altura - giro_z + giro_y );
	motor[1] = ( altura + giro_z - giro_x );
	motor[2] = ( altura + giro_z + giro_x );
	motor[3] = ( altura - giro_z - giro_y );

	if(joystick[4] > 0 && altura > 30) { // Emergency switch
		motors_set1(motor[0]);
		motors_set2(motor[1]);
		motors_set3(motor[2]);
		motors_set4(motor[3]);
	} else {
		motors_set1(0);
		motors_set2(0);
		motors_set3(0);
		motors_set4(0);
	}

TEST_PIN2_OFF;
}

