#include "joystick.h"
#include "timer1.h"
#include <stdio.h> 

s16 joystick[8]={0,0,0,0,0,0,0,0};
s16 joystick2[8]={0,0,0,0,0,0,0,0};

void joystick_setup() {
	joystick[0]=0;
	joystick[1]=0;
	joystick[2]=0;
	joystick[3]=0;
	joystick[4]=0;
	joystick[5]=0;
	joystick[6]=0;
	joystick[7]=0;
}

void joystick_exti(u8 n, u16 time) {
    static u16 previous_time=0;
    u16 time_delta;

//    if(n==7) // 1,4 funciona    5,6,7 raro
//        printf("%d:%d\r\n", ((estado)?1:0), time);
    joystick2[n]++;
    //if(estado) {
	if(time >= previous_time) {
        	time_delta = time - previous_time;
	} else {
        	time_delta = TIMER1_PERIOD - (previous_time - time);
	}
	if(n==0) n=8;
     	joystick[n-1] = (time_delta - 2000)/2;
        previous_time = time;
    //} else {
    //    previous_time = time;
    //}

}

/*
void joystick_exti(u8 n, u8 estado, u16 time) {
    static u16 previous_time[8]={0,0,0,0,0,0,0,0};
    u16 time_delta;

//    if(n==7) // 1,4 funciona    5,6,7 raro
//        printf("%d:%d\r\n", ((estado)?1:0), time);
    joystick2[n]++;
    if(!estado) {
	if(time >= previous_time[0]) {
        	time_delta = time - previous_time[0];
	} else {
        	time_delta = TIMER1_PERIOD - (previous_time[0] - time);
	}
        joystick[n] = (time_delta - 2000)/2;
        previous_time[0] = time;
    } else {
        previous_time[0] = time;
    }

}
*/

