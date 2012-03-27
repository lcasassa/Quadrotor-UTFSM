#ifndef GYRO_FUTABA_H
#define GYRO_FUTABA_H

#include <libopencm3/cm3/common.h>

extern s16 gyro_futaba;

void gyro_futaba_setup();
inline void gyro_futaba_timer_capture(u32 time);

#endif
