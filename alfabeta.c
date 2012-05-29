#include "alfabeta.h"
#include <stdio.h>

struct AlfaBeta alfabeta[2];

void alfabeta_setup() {

//	alfabeta[0].P = 0.6;
//	alfabeta[0].I = 0.005;
	alfabeta[0].I_sum = 0;
	alfabeta[0].I_max = 10;
	alfabeta[0].I_min = -10;
	alfabeta[0].dt = 0.002;

//	alfabeta[1].P = 0.6;
//	alfabeta[1].I = 0.005;
	alfabeta[1].I_sum = 0;
	alfabeta[1].I_max = 10;
	alfabeta[1].I_min = -10;
	alfabeta[1].dt = 0.002;

}

float alfabeta_loop(struct AlfaBeta *a, float angle2, float *gyro) {
	float e;

	a->angle += *gyro * a->dt;
	e = (angle2 - a->angle)/10000;
	a->I_sum += e/1000;

	if((a->I != 0) && (a->I*a->I_sum) < a->I_min) {
		a->I_sum = a->I_min/a->I;
	} else if((a->I != 0) && (a->I*a->I_sum) > a->I_max) {
		a->I_sum = a->I_max/a->I;
	}

	//*gyro = *gyro + ((a->P * e) + (a->I * a->I_sum)) / a->dt;

	a->angle += (a->P * e) + (a->I * a->I_sum);

	return a->angle;
}

