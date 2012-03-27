#ifndef ESTABILIZADOR_H
#define ESTABILIZADOR_H

#include <libopencm3/cm3/common.h>

struct ESTABILIZADOR {
    u16 offset;
    float ganancia; // angulo a actuador
};

extern struct ESTABILIZADOR estabilizador[2];

void estabilizador_setup();
void estabilizador_loop(u16 angle_x, u16 angle_y);

#endif
