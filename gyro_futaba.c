#include "gyro_futaba.h"
#include "timer3.h"
#include <stdio.h> 
#include <libopencm3/stm32/exti.h>

s16 gyro_futaba_input = 0;
//s16 gyro_futaba2=0;

void gyro_futaba_setup() {
	gyro_futaba_input = 0;
}

s16 gyro_futaba(u16 value) { // -1000 to 1000
	s16 gyro;
	PWM5 = value + 3000; // 2000 to 4000
	gyro = gyro_futaba_input;
	gyro /= 2; // Less range gain for futaba gyro
	return gyro;

}

inline void gyro_futaba_timer_capture(u32 timer) {
	if(timer > 4100) return;
	if(timer < 1900) return;
	gyro_futaba_input = (s16)timer-3000; // centered on 1.5ms
}

