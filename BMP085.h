#ifndef BMP085_H
#define BMP085_H

#include <libopencm3/cm3/common.h>

extern float temperature;
extern float pressure;
extern float altitude;

void BMP085_setup();
void BMP085_getValues();

/* ************************ Register map for the BMP085 ****************************/
#define BMP_ADDR	(0xEE>>1) //0xD0 if tied low, 0xD2 if tied high


#endif
