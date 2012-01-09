#include "pid.h"

struct_pid pid[2];

void pid_setup(struct_pid *p) {
	p->P       = 0;
	p->D_ref   = 0;
	p->D_med   = 0;
	p->ref_old = 0;
	p->med_old = 0;
}

float pid_update(struct_pid *p, float ref, float med) {
	float u;
	u = p->P*(ref - med) + p->D_med*(med - p->med_old) + p->D_ref*(ref - p->ref_old);
	p->med_old = med;
	p->ref_old = ref;
	return u;
}

