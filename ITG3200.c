#include "ITG3200.h"
#include "i2c.h"
#include <stdio.h>

//#define ITG3200_PRINTF_ERROR 1
#define ITG3200_PRINTF_DEBUG 1

float gyroscope[3] = {0,0,0};
float gyroscope_offset[3] = {0, 0, 0};
float gyroscope_gain[3] = {1, 1, 1};
s8 volatile gyroscope_is_calibrated = 0;
s8 volatile gyroscope_is_calibration_checked = 0;

void ITG3200_setup() {
	int return_value;
	gyroscope_is_calibrated = 0;
	gyroscope_is_calibration_checked = 0;


	//Set internal clock to 1kHz with 188Hz LPF and Full Scale(+-2000) to 3 for proper operation
//	return_value = i2c_write(ITG_ADDR, DLPF_FS, DLPF_FS_SEL_0|DLPF_FS_SEL_1);//|DLPF_CFG_0);
	//return_value = i2c_write(ITG_ADDR, DLPF_FS, DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0|DLPF_CFG_1|DLPF_CFG_2);
	return_value = i2c_write(ITG_ADDR, DLPF_FS, DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0|DLPF_CFG_1);
#ifdef ITG3200_PRINTF_ERROR
	if(return_value < 1) {
		printf("%s (%d): i2c_write(ITG_ADDR, DLPF_FS, DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_1): return_value: %d\r\n", __FILE__, __LINE__, return_value);
	}
#endif

	//Set sample rate divider for 333 Hz operation
	return_value = i2c_write(ITG_ADDR, SMPLRT_DIV, 3);	//Fsample = Fint / (divider + 1) where Fint is 1kHz
#ifdef ITG3200_PRINTF_ERROR
	if(return_value < 1) {
		printf("%s (%d): i2c_write(ITG_ADDR, SMPLRT_DIV, 4): return_value: %d\r\n", __FILE__, __LINE__, return_value);
	}
#endif
	
	//Setup the interrupt to trigger when new data is ready.
	return_value = i2c_write(ITG_ADDR, INT_CFG, 0);// INT_CFG_RAW_RDY_EN | INT_CFG_ITG_RDY_EN);
#ifdef ITG3200_PRINTF_ERROR
	if(return_value < 1) {
		printf("%s (%d): 2c_write(ITG_ADDR, INT_CFG, 0): return_value: %d\r\n", __FILE__, __LINE__, return_value);
	}
#endif
	
	//Select X gyro PLL for clock source
	return_value = i2c_write(ITG_ADDR, PWR_MGM, PWR_MGM_CLK_SEL_0);
#ifdef ITG3200_PRINTF_ERROR
	if(return_value < 1) {
		printf("%s (%d): i2c_write(ITG_ADDR, PWR_MGM, PWR_MGM_CLK_SEL_0): return_value: %d\r\n", __FILE__, __LINE__, return_value);
	}
#endif

}

void ITG3200_getValues() {
	int return_value;
	float gyroscope_tmp=0;
	char value[2];

	return_value = i2c_read(ITG_ADDR, GYRO_XOUT_H, value, 2);
#ifdef ITG3200_PRINTF_ERROR
        if(return_value < 1) {
                printf("%s (%d): i2c_read(ITG_ADDR, GYRO_XOUT_H, value, 2): return_value: %d\r\n", __FILE__, __LINE__, return_value);
        }
#endif
        if(return_value == 1) {
		gyroscope_tmp = (float)(s16)(((s16)value[0]<<8) | value[1]);
		if(gyroscope_is_calibrated) {
			gyroscope_tmp -= gyroscope_offset[0];
			gyroscope_tmp *= gyroscope_gain[0];
		}
		gyroscope[0] = gyroscope_tmp;
	}

	return_value = i2c_read(ITG_ADDR, GYRO_YOUT_H, value, 2);
#ifdef ITG3200_PRINTF_ERROR
        if(return_value < 1) {
                printf("%s (%d): i2c_read(ITG_ADDR, GYRO_YOUT_H, value, 2): return_value: %d\r\n", __FILE__, __LINE__, return_value);
        }
#endif
        if(return_value == 1) {
		gyroscope_tmp = (float)(s16)(((s16)value[0]<<8) | value[1]);
		if(gyroscope_is_calibrated) {
			gyroscope_tmp -= gyroscope_offset[1];
			gyroscope_tmp *= gyroscope_gain[1];
		}
		gyroscope[1] = gyroscope_tmp;
	}

	return_value = i2c_read(ITG_ADDR, GYRO_ZOUT_H, value, 2);
#ifdef ITG3200_PRINTF_ERROR
        if(return_value < 1) {
                printf("%s (%d): i2c_read(ITG_ADDR, GYRO_ZOUT_H, value, 2): return_value: %d\r\n", __FILE__, __LINE__, return_value);
        }
#endif
        if(return_value == 1) {
		gyroscope_tmp = (float)(s16)(((s16)value[0]<<8) | value[1]);
		if(gyroscope_is_calibrated) {
			gyroscope_tmp -= gyroscope_offset[2];
			gyroscope_tmp *= gyroscope_gain[2];
		}
		gyroscope[2] = gyroscope_tmp;
	}
}

void ITG3200_calibrate() {
	static s8 running = 0;
	static int count=0;
	s8 i;

	// Start
	if(!running) {
#ifdef ITG3200_PRINTF_DEBUG
		printf("Starting calibration\r\n");
#endif
		running = 1;
		count=0;
		gyroscope_is_calibration_checked = 0;
		gyroscope_is_calibrated = 0;
		for(i=0; i<3; i++) {
			gyroscope_offset[i] = 0;
		}
		return;
	}

	// Calculating a simple average
	for(i=0; i<3; i++) {
		gyroscope_offset[i] += gyroscope[i];
	}

	count++;

	// End
	if(count >= 1000) {
		for(i=0; i<3; i++) {
			gyroscope_offset[i] /= 1000;
		}
#ifdef ITG3200_PRINTF_DEBUG
		printf("Stoping calibration: %d %d %d\r\n", (int)gyroscope_offset[0], (int)gyroscope_offset[1], (int)gyroscope_offset[2]);
#endif
		gyroscope_is_calibrated = 1;
		running = 0;
		return;
	}

}

void ITG3200_check_calibration() {
	static s32 integral_tmp[3] = {0,0,0};
	static s8 running = 0;
	static int count=0;
	s8 i;

	// Start
	if(!running) {
		if(!gyroscope_is_calibrated) return;
#ifdef ITG3200_PRINTF_DEBUG
		printf("Starting calibration check: %d %d %d\r\n", (int)gyroscope[0], (int)gyroscope[1], (int)gyroscope[2]);
#endif
		running = 1;
		count=0;
		gyroscope_is_calibration_checked = 0;
		for(i=0; i<3; i++) {
			integral_tmp[i] = 0;
		}
		return;
	}

	// Calculating a simple average
	for(i=0; i<3; i++) {
		integral_tmp[i] += gyroscope[i];
	}

	count++;

	// End
	if(count >= CALIBRATION_CHECK_TIME/2) {
#ifdef ITG3200_PRINTF_DEBUG
		printf("Stoping calibration check\r\n");
#endif
		for(i=0; i<3; i++) {
			if(!(-CALIBRATION_THRESHOLD < integral_tmp[i] && integral_tmp[i] < CALIBRATION_THRESHOLD)) {
				gyroscope_is_calibrated = 0;
				running = 0;
#ifdef ITG3200_PRINTF_DEBUG
				printf("Bad calibration: %d %d %d\r\n", (int)integral_tmp[0], (int)integral_tmp[1], (int)integral_tmp[2]);
#endif
				return;
			}
		}
#ifdef ITG3200_PRINTF_DEBUG
		printf("Good calibration: %d %d %d\r\n", (int)integral_tmp[0], (int)integral_tmp[1], (int)integral_tmp[2]);
#endif
		gyroscope_is_calibration_checked = 1;
		running = 0;
		return;
	}

}



