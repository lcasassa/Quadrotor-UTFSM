#include "alfabeta.h"
#include <stdio.h>

struct AlfaBeta alfabeta[2];

void alfabeta_setup() {

	alfabeta[0].beta = 0.4;
	alfabeta[0].dt = 0.002;
	alfabeta[1].beta = 0.4;
	alfabeta[1].dt = 0.002;

}

float alfabeta_loop(struct AlfaBeta *a, float angle2, float gyro) {
	float e;

	a->angle += gyro * a->dt;
	e = angle2 - a->angle;
	a->angle += (a->beta * e) * a->dt;

	return a->angle;
}

