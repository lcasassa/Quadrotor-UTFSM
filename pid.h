#ifndef PID_H
#define PID_H

#include <libopencm3/cm3/common.h>

typedef struct {
	float P;
	float I;
	float D;
	float I_sum;
	float I_sum_min;
	float I_sum_max;
	float e_old;
} struct_pid;

extern struct_pid pid[4];

void pid_setup(struct_pid *p);
float pid_update(struct_pid *p, float ref, float med);
float pid_reset(struct_pid *p, float ref, float med);

#endif
