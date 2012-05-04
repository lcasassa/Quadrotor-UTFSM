#ifndef ALFABETA_H
#define ALFABETA_H

struct AlfaBeta {
	float angle;
	float P;
	float I;
	float I_sum;
	float I_max;
	float I_min;
	float dt;
};

extern struct AlfaBeta alfabeta[2];

void alfabeta_setup();
float alfabeta_loop(struct AlfaBeta *a, float angle, float *gyro);

#endif
