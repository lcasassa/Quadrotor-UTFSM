#include "motors.h"

void motors_setup() {
	motors_set1(0);
	motors_set2(0);
	motors_set3(0);
	motors_set4(0);
}

// value: 0 to (MOTORS_MAX-MOTORS_MIN)
inline u16 motors_max_min(s32 value) {
	value += MOTORS_MIN;
	if(value > MOTORS_MAX) value = MOTORS_MAX;
	else if(MOTORS_MIN > value) value = MOTORS_MIN;
	return value;
}

