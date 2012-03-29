#include "joystick.h"
#include "timer1.h"
#include <stdio.h> 

s16 joystick[8]={0,0,0,0,0,0,0,0};
s16 joystick_update_count[8]={0,0,0,0,0,0,0,0};

void joystick_setup() {
    int i;
    for(i=0; i<8; i++) {
	joystick[i]=0;
	joystick_update_count[i]=0;
    }
}

void joystick_exti(u8 n, u16 time) {
    static u16 previous_time=0;
    u16 time_delta;

    joystick_update_count[n]++;
    if(time >= previous_time) {
       	time_delta = time - previous_time;
    } else {
       	time_delta = TIMER1_PERIOD - (previous_time - time);
    }
    if(n==0) n=8;
    joystick[n-1] = (time_delta - 2000)/2;
    previous_time = time;
}

