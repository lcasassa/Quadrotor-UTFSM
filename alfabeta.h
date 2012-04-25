#ifndef ALFABETA_H
#define ALFABETA_H

struct AlfaBeta {
	float angle;
	float beta;
	float dt;
};

extern struct AlfaBeta alfabeta[2];

void alfabeta_setup();
float alfabeta_loop(struct AlfaBeta *a, float angle, float gyro);

#endif
