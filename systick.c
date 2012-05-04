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

#define DT 0.002
void sys_tick_handler(void)
{
	static u16 count=0;
	static float omega_ref[2] = {0.0, 0.0};
	static float dt_angle_z = 0.0;
	float gyro[2];
	float angle_tmp[2];
	s32 altura=0, giro_x=0, giro_y=0, giro_z=0;
	s32 motor[4];

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
		
		return;
	}

	gyro[0] = gyroscope[0]*1.15/14.375;
	gyro[1] = gyroscope[1]*1.15/14.375;

#if 1
	
	dt_angle_z = gyroscope[2]*1.15/14.375*DT*M_PI/180.0;

		angle_tmp[0] = alfabeta[0].angle;
		angle_tmp[1] = alfabeta[1].angle;
		alfabeta[0].angle = angle_tmp[0]*cos(dt_angle_z) - angle_tmp[1]*sin(dt_angle_z);
		alfabeta[1].angle = angle_tmp[1]*cos(dt_angle_z) + angle_tmp[0]*sin(dt_angle_z);
	
#endif

	angle[0] = alfabeta_loop(&alfabeta[0],  180.0*atan2((double)-acelerometer[0],(double)acelerometer[2])/M_PI, &gyro[1]);
	angle[1] = alfabeta_loop(&alfabeta[1], -180.0*atan2((double)-acelerometer[1],(double)acelerometer[2])/M_PI, &gyro[0]);

	angle_gyro[0] += gyro[1]*DT;
	angle_gyro[1] += gyro[0]*DT;

	count++;
	if ((count%(10/2)) == 0) { // 10 ms
		estabilizador_loop(0,0);
	}
	if ((count%(50/2)) == 0) { // 50 ms
		omega_ref[0] = pid_update(&pid[2], ((float)joystick[0])/10, angle[1]);
		omega_ref[1] = pid_update(&pid[3], ((float)joystick[1])/10, angle[0]);
		omega_ref[0] *= -1;
		omega_ref[1] *= -1;
		count = 0;
	}

	giro_z = gyro_futaba(joystick[3]);

	altura = joystick[2];
	if(pid[2].P == 0.0 && pid[3].P == 0.0 ) {
		giro_x = (int)(pid_update(&pid[0], (float)joystick[0], gyro[0]));
		giro_y = (int)(pid_update(&pid[1], (float)joystick[1], gyro[1]));
	} else {
		giro_x = (int)(pid_update(&pid[0], omega_ref[0], gyro[0]));
		giro_y = (int)(pid_update(&pid[1], omega_ref[1], gyro[1]));
	}

	motor[0] = ( altura - giro_z + giro_y );
	motor[1] = ( altura + giro_z - giro_x );
	motor[2] = ( altura + giro_z + giro_x );
	motor[3] = ( altura - giro_z - giro_y );

//	if (count == 0) { // 50 ms
//		printf("%d %d %d %d\r\n", (int)motor[0], (int)motor[1], (int)motor[2], (int)motor[3]);
//		printf("s%5d %5d %5d\r\n", (int)((float)joystick[1])/30, (int)angle[0], (int)omega_ref[1]);
//	}

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

}

