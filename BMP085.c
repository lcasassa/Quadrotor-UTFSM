#include "BMP085.h"
#include "i2c1.h"
#include <math.h>
#include <stdio.h>

#define BMP085_PRINTF_ERROR 1
#define BMP085_PRINTF_DEBUG 1

float temperature = 0;
float pressure = 0;
float altitude = 0;

short ac1;
short ac2; 
short ac3; 
unsigned short ac4;
unsigned short ac5;
unsigned short ac6;
short b1; 
short b2;
short mb;
short mc;
short md;

short bmp085ReadShort(unsigned char address)
{
	int return_value;
	char value[2];
	
	return_value = i2c1_read(BMP_ADDR, address, value, 2);
#ifdef BMP085_PRINTF_ERROR
        if(return_value < 1) {
                printf("%s (%d): i2c1_read(ITG_ADDR, GYRO_XOUT_H, value, 2): return_value: %d\r\n", __FILE__, __LINE__, return_value);
        }
#endif


	return (value[0]<<8) | value[1];
}


void BMP085_setup() {

	printf("\r\nCalibration Information:\r\n");
	printf("------------------------\r\n");
	ac1 = bmp085ReadShort(0xAA);
	ac2 = bmp085ReadShort(0xAC);
	ac3 = bmp085ReadShort(0xAE);
	ac4 = bmp085ReadShort(0xB0);
	ac5 = bmp085ReadShort(0xB2);
	ac6 = bmp085ReadShort(0xB4);
	b1 = bmp085ReadShort(0xB6);
	b2 = bmp085ReadShort(0xB8);
	mb = bmp085ReadShort(0xBA);
	mc = bmp085ReadShort(0xBC);
	md = bmp085ReadShort(0xBE);
	
	printf("\tAC1 = %d\r\n", ac1);
	printf("\tAC2 = %d\r\n", ac2);
	printf("\tAC3 = %d\r\n", ac3);
	printf("\tAC4 = %d\r\n", ac4);
	printf("\tAC5 = %d\r\n", ac5);
	printf("\tAC6 = %d\r\n", ac6);
	printf("\tB1 = %d\r\n", b1);
	printf("\tB2 = %d\r\n", b2);
	printf("\tMB = %d\r\n", mb);
	printf("\tMC = %d\r\n", mc);
	printf("\tMD = %d\r\n", md);
	printf("------------------------\r\n\n");

}

#define BMP085_DELAY for(i=0; i<720000; i++) __asm__("nop") // 10ms*72MHz = 720000
void BMP085_getValues() {
	int return_value;
	long int i;
	long x1, x2, b5, b6, x3, b3, p;
	unsigned long b4, b7;
	long pressure_tmp;
/*
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
			gyroscope_tmp -= gyroscope_offset[1];
			gyroscope_tmp *= gyroscope_gain[1];
		}
		gyroscope[1] = gyroscope_tmp;
	}
*/

	return_value = i2c1_write(BMP_ADDR, 0xF4, 0x2E);
#ifdef BMP085_PRINTF_ERROR
	if(return_value < 1) {
		printf("%s (%d): i2c1_write(BMP_ADDR, DLPF_FS, DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_1): return_value: %d\r\n", __FILE__, __LINE__, return_value);
	}
#endif

	
	BMP085_DELAY;	// max time is 4.5ms
	
	x1 = ((long)bmp085ReadShort(0xF6) - ac6) * ac5 >> 15;
	x2 = ((long) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	temperature = (float)((b5 + 8) >> 4);

	BMP085_DELAY;	// max time is 4.5ms

	return_value = i2c1_write(BMP_ADDR, 0xF4, 0x34);
#ifdef BMP085_PRINTF_ERROR
	if(return_value < 1) {
		printf("%s (%d): i2c1_write(BMP_ADDR, 0xF4, 0x34): return_value: %d\r\n", __FILE__, __LINE__, return_value);
	}
#endif

	
	BMP085_DELAY;	// max time is 4.5ms
	
	pressure_tmp = bmp085ReadShort(0xF6);
	pressure_tmp &= 0x0000FFFF;
	
	b6 = b5 - 4000;
	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ac2 * b6 >> 11;
	x3 = x1 + x2;
	b3 = (((int32_t) ac1 * 4 + x3) + 2)/4;
	x1 = ac3 * b6 >> 13;
	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
	b7 = ((unsigned long) pressure_tmp - b3) * (50000 >> 0);
	p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	pressure = (float)(p + ((x1 + x2 + 3791) >> 4));


	  // Add this into loop(), after you've calculated the pressure
  altitude = (float)44330 * (1 - pow(((float) pressure/101325), 0.190295));

}



