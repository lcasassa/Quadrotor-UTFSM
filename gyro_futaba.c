#include "gyro_futaba.h"
#include "timer1.h"
#include "timer2.h"
#include <stdio.h> 
#include <libopencm3/stm32/exti.h>

s16 gyro_futaba=0;
//s16 gyro_futaba2=0;

void gyro_futaba_setup() {
	gyro_futaba=0;
}

inline void gyro_futaba_timer_capture(u32 timer) {
	gyro_futaba = timer-2000;
}

/*
inline void gyro_futaba_exti(u8 estado, u16 time) {
    static u16 previous_time=0;
    static u8 flag=0;
    static u32 previous_timer2_counter;
    static s8 filtrado=0;
    s16 tmp;
    u16 time_delta;
    u32 timer2_counter_delta;
    gyro_futaba2++;
    if(estado) {
	flag=1;
        previous_time = time;

//	printf("u");
	previous_timer2_counter = timer2_counter;
    } else {
	if(timer2_counter >= previous_timer2_counter) {
		timer2_counter_delta = timer2_counter - previous_timer2_counter;
	} else {
		timer2_counter_delta = (u32)0xFFFFFFFF - (previous_timer2_counter - timer2_counter);
	}
	if(!(2 <= timer2_counter_delta && timer2_counter_delta <= 4)) {
		return;
	}
	flag=0;
	if(time >= previous_time) {
        	time_delta = time - previous_time;
	} else {
        	time_delta = TIMER1_PERIOD - (previous_time - time);
	}
     	tmp = (time_delta - 2000)/2;
	if((3 < filtrado) || (-50 < gyro_futaba-tmp && gyro_futaba-tmp < 50)) {
     		gyro_futaba = tmp;
		filtrado=0;
//	   	printf("E%d\t%d\r\n", tmp, (int)timer2_counter_delta);
	} else {
	//   	printf("F%d\t%d\r\n", tmp, (int)timer2_counter_delta);
		filtrado++;
	}
	//printf("d\n");
    }

}
*/

