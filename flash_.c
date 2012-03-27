#include <libopencm3/stm32/f1/flash.h>
#include "flash_.h"
#include "pid.h"
#include "estabilizador.h"
#include <stdio.h>

const u32 fla[256] __attribute__ ((section (".text\n\t#"), aligned(1024)));

#define SAVE(v) value = (u32*)&v; \
                flash_program_word((u32)&fla[i++], *value);
#define RESTORE(v, type, tmp) tmp = (type*)&fla[i++]; \
                              v     = *tmp;

void flash_load() {
	int i=0;
	float *tmp_f;
	u16 *tmp_u16;

	RESTORE(pid[0].P, float, tmp_f);
	RESTORE(pid[0].I, float, tmp_f);
	RESTORE(pid[0].D, float, tmp_f);
	RESTORE(pid[0].I_sum_min, float, tmp_f);
	RESTORE(pid[0].I_sum_max, float, tmp_f);

	RESTORE(pid[1].P, float, tmp_f);
	RESTORE(pid[1].I, float, tmp_f);
	RESTORE(pid[1].D, float, tmp_f);
	RESTORE(pid[1].I_sum_min, float, tmp_f);
	RESTORE(pid[1].I_sum_max, float, tmp_f);

	RESTORE(estabilizador[0].offset, u16, tmp_u16);
	RESTORE(estabilizador[0].ganancia, float, tmp_f);

	RESTORE(estabilizador[1].offset, u16, tmp_u16);
	RESTORE(estabilizador[1].ganancia, float, tmp_f);
}

#define SAVE(v) value = (u32*)&v; \
                flash_program_word((u32)&fla[i++], *value);


void flash_save() {
	int i=0;
	u32 *value;

	printf("Salvando..\r\n");

	flash_unlock();
	flash_erase_page((u32)&fla[0]);

	SAVE(pid[0].P);
	SAVE(pid[0].I);
	SAVE(pid[0].D);
	SAVE(pid[0].I_sum_min);
	SAVE(pid[0].I_sum_max);

	SAVE(pid[1].P);
	SAVE(pid[1].I);
	SAVE(pid[1].D);
	SAVE(pid[1].I_sum_min);
	SAVE(pid[1].I_sum_max);

	SAVE(estabilizador[0].offset);
	SAVE(estabilizador[0].ganancia);

	SAVE(estabilizador[1].offset);
	SAVE(estabilizador[1].ganancia);

	flash_lock();

	printf("Salvado!\r\n");
}

