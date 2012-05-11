#include "joystick.h"
#include "timer1.h"
#include <stdio.h> 

s16 joystick[8]={0,0,0,0,0,0,0,0};
s16 joystick_update_count[8]={0,0,0,0,0,0,0,0};
s16 joystick_offset[8]={0,0,0,0,0,0,0,0};

void joystick_setup() {
    int i;
    for(i=0; i<8; i++) {
	joystick[i]=0;
	joystick_update_count[i]=0;
	joystick_offset[i] = 77;
    }
}

void joystick_exti(u8 n, u16 time, u8 edge) {
    static u16 previous_time[8] = {0,0,0,0,0,0,0,0};
    static u8 old_edge[8] = {0,0,0,0,0,0,0,0};
    u16 time_delta;

    joystick_update_count[n]++;

    if(!edge && old_edge[n]) { 
        if(time >= previous_time[n]) {
       	    time_delta = time - previous_time[n];
        } else {
            time_delta = TIMER1_PERIOD - (previous_time[n] - time);
        }
        if(n==2) // throttle: 1ms to 2ms
	    joystick[n] = (time_delta - 2000) + joystick_offset[n]; // 0 to 2000
        else // all others centered on 1.5ms
            joystick[n] = (time_delta - 3000) + joystick_offset[n]; // -1000 a 1000
    } else {
    	previous_time[n] = time;
    }

    old_edge[n] = edge;
}

/*
// in order
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
    if(n==3) // throttle: 1ms to 2ms
	joystick[n-1] = (time_delta - 2000); // 0 to 2000
    else // all others centered on 1.5ms
	joystick[n-1] = (time_delta - 3000); // -1000 a 1000
    previous_time = time;
}
*/

