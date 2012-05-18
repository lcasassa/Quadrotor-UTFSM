#include "ADXL345.h"
#include "i2c2.h"
#include <stdio.h>

//#define ADXL_PRINTF_ERROR 1

s16 acelerometer[3] = {0,0,0};

void ADXL345_setup() {
	int return_value;

        //Put the accelerometer in MEASURE mode
        return_value = i2c2_write(ADXL_ADDR, POWER_CTL, MEASURE);
#ifdef ADXL_PRINTF_ERROR
	if(return_value < 1) {
		printf("%s (%d): i2c2_write(ADXL_ADDR, POWER_CTL, MEASURE): return_value: %d\r\n", __FILE__, __LINE__, return_value);
	}
#endif
        //Set the Range to full range
        return_value = i2c2_write(ADXL_ADDR, DATA_FORMAT, FULL_RES);//RANGE_0);
#ifdef ADXL_PRINTF_ERROR
	if(return_value < 1) {
		printf("%s (%d): i2c2_write(ADXL_ADDR, DATA_FORMAT, FULL_RES): return_value: %d\r\n", __FILE__, __LINE__, return_value);
	}
#endif

        //Set the Output Data Rate to 400 Hz -> BW 200Hz
        i2c2_write(ADXL_ADDR, BW_RATE, 0xC);

	// Enable interrupt when data ready
        return_value = i2c2_write(ADXL_ADDR, INT_ENABLE, 0);
#ifdef ADXL_PRINTF_ERROR
	if(return_value < 1) {
		printf("%s (%d): i2c2_write(ADXL_ADDR, INT_ENABLE, 0): return_value: %d\r\n", __FILE__, __LINE__, return_value);
	}
#endif
	

}

void ADXL345_getValues() {
	int return_value;
	s16 value;

        return_value = i2c2_read(ADXL_ADDR, DATAX0, (char*)&value, 2);
#ifdef ADXL_PRINTF_ERROR
        if(return_value < 1) {
                printf("%s (%d): i2c2_read(ADXL_ADDR, DATAX0, (char*)&value, 2): return_value: %d\r\n", __FILE__, __LINE__, return_value);
        }
#endif
	if(return_value == 1) acelerometer[0] = value;

        return_value = i2c2_read(ADXL_ADDR, DATAY0, (char*)&value, 2);
#ifdef ADXL_PRINTF_ERROR
        if(return_value < 1) {
                printf("%s (%d): i2c2_read(ADXL_ADDR, DATAY0, (char*)&value, 2): return_value: %d\r\n", __FILE__, __LINE__, return_value);
        }
#endif
        if(return_value == 1) acelerometer[1] = value;
        
        return_value = i2c2_read(ADXL_ADDR, DATAZ0, (char*)&value, 2);
#ifdef ADXL_PRINTF_ERROR
        if(return_value < 1) {
                printf("%s (%d): i2c2_read(ADXL_ADDR, DATAZ0, (char*)&value, 2): return_value: %d\r\n", __FILE__, __LINE__, return_value);
        }
#endif
        if(return_value == 1) acelerometer[2] = value;
}

float ADXL345_calibration() {
	
}

