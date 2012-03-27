#include "estabilizador.h"
#include "timer3.h"
#include "kalman.h"
#include <math.h>

struct ESTABILIZADOR estabilizador[2];

void estabilizador_setup() {
	estabilizador[0].offset=0;
	estabilizador[0].ganancia=1;
	estabilizador[1].offset=0;
	estabilizador[1].ganancia=1;
}

void estabilizador_loop(u16 angle_x, u16 angle_y) {
	//PWM5 = estabilizador[0].ganancia*(-k[1].angle/M_PI*3600 - k[0].angle/M_PI*3600) + 1500*2 + angle_x + estabilizador[0].offset;
	//PWM6 = estabilizador[1].ganancia*( k[1].angle/M_PI*3600 - k[0].angle/M_PI*3600) + 1500*2 + angle_y + estabilizador[1].offset;
//	PWM5 = estabilizador[0].ganancia*(-k[1].angle/M_PI*3600 - k[0].angle/M_PI*3600) + 1500*2 + angle_x + estabilizador[0].offset;
	PWM6 = estabilizador[1].ganancia*( k[1].angle/M_PI*3600 - k[0].angle/M_PI*3600) + 1500*2 + angle_y + estabilizador[1].offset;
}

