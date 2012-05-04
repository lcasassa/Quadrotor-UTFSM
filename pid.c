#include "pid.h"
#include <stdio.h>

struct_pid pid[4];

void pid_setup(struct_pid *p) {
	p->P         = 0;
	p->I         = 0;
	p->D         = 0;
	p->I_sum     = 0;
	p->I_sum_min = -100;
	p->I_sum_max = 100;
	p->e_old     = 0;
}

float pid_update(struct_pid *p, float ref, float med) {
	float u, e;
	e = med - ref;

	p->I_sum += e/100;
	p->I_sum_min = -100;
	p->I_sum_max = 100;

	if((p->I != 0) && (p->I*p->I_sum) < p->I_sum_min) {
		p->I_sum = p->I_sum_min/p->I;
	} else if((p->I != 0) && (p->I*p->I_sum) > p->I_sum_max) {
		p->I_sum = p->I_sum_max/p->I;
	}

	u = p->P*e - p->D*(e - p->e_old) + p->I*p->I_sum;

	p->e_old = e;
	return u;
}

