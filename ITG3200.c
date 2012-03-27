#include "ITG3200.h"
#include "i2c.h"
#include <stdio.h>

//#define ITG3200_PRINTF_ERROR 1

s16 gyroscope[3] = {0,0,0};

void ITG3200_setup() {
	int return_value;

	//Set internal clock to 1kHz with 188Hz LPF and Full Scale(+-2000) to 3 for proper operation
//	return_value = i2c_write(ITG_ADDR, DLPF_FS, DLPF_FS_SEL_0|DLPF_FS_SEL_1);//|DLPF_CFG_0);
	return_value = i2c_write(ITG_ADDR, DLPF_FS, DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0|DLPF_CFG_1|DLPF_CFG_2);
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
	char value[2];

	return_value = i2c_read(ITG_ADDR, GYRO_XOUT_H, value, 2);
#ifdef ITG3200_PRINTF_ERROR
        if(return_value < 1) {
                printf("%s (%d): i2c_read(ITG_ADDR, GYRO_XOUT_H, value, 2): return_value: %d\r\n", __FILE__, __LINE__, return_value);
        }
#endif
        if(return_value == 1) gyroscope[0] = ((s16)value[0]<<8) | value[1];

	return_value = i2c_read(ITG_ADDR, GYRO_YOUT_H, value, 2);
#ifdef ITG3200_PRINTF_ERROR
        if(return_value < 1) {
                printf("%s (%d): i2c_read(ITG_ADDR, GYRO_YOUT_H, value, 2): return_value: %d\r\n", __FILE__, __LINE__, return_value);
        }
#endif
        if(return_value == 1) gyroscope[1] = ((s16)value[0]<<8) | value[1];

	return_value = i2c_read(ITG_ADDR, GYRO_ZOUT_H, value, 2);
#ifdef ITG3200_PRINTF_ERROR
        if(return_value < 1) {
                printf("%s (%d): i2c_read(ITG_ADDR, GYRO_ZOUT_H, value, 2): return_value: %d\r\n", __FILE__, __LINE__, return_value);
        }
#endif
        if(return_value == 1) gyroscope[2] = ((s16)value[0]<<8) | value[1];

}

