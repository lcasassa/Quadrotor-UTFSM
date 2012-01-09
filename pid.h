#ifndef PID_H
#define PID_H

typedef struct {
	float P;
	float D_ref;
	float D_med;
	float ref_old;
	float med_old;
} struct_pid;

extern struct_pid pid[2];

void pid_setup(struct_pid *p);
float pid_update(struct_pid *p, float ref, float med);

#endif
