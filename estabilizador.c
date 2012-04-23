#include "estabilizador.h"
#include "timer2.h"
#include "kalman.h"
#include <math.h>
#include "main.h"

struct ESTABILIZADOR estabilizador[2];

void estabilizador_setup() {
	estabilizador[0].offset=180;
	estabilizador[0].ganancia=0.450;
	estabilizador[1].offset=-200;
	estabilizador[1].ganancia=1;
}

void estabilizador_loop(u16 angle_x, u16 angle_y) {
	//PWM5 = estabilizador[0].ganancia*(-k[1].angle/M_PI*3600 - k[0].angle/M_PI*3600) + 1500*2 + angle_x + estabilizador[0].offset;
	//PWM6 = estabilizador[1].ganancia*( k[1].angle/M_PI*3600 - k[0].angle/M_PI*3600) + 1500*2 + angle_y + estabilizador[1].offset;
	PWM7 = estabilizador[0].ganancia*( -angle[1]*20 - angle[0]*20) + 1500*2 + angle_x + estabilizador[0].offset;
	PWM8 = estabilizador[1].ganancia*( -angle[1]*20 + angle[0]*20) + 1500*2 + angle_y + estabilizador[1].offset;
}

