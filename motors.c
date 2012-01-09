#include "motors.h"

void motors_setup() {
	motors_set(0, 0);
	motors_set(1, 0);
	motors_set(2, 0);
	motors_set(3, 0);
}

void motors_set(u8 number, s16 value) {

	// limite de value 2000 y 4000
	if(1000 < value)
		value = 1000;
	if(value <= 0)
		value = 0;

//	value = -value;
	value *= 2;
	value += 2000;

	switch(number) {
	case 0:
	    PWM1 = value;
		break;
	case 1:
	    PWM2 = value;
		break;
	case 2:
	    PWM3 = value;
		break;
	case 3:
	    PWM4 = value;
		break;
	default:
		break;

	}
}
