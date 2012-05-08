#include <libopencm3/stm32/f1/flash.h>
#include "flash_.h"
#include "pid.h"
#include "ITG3200.h"
#include "estabilizador.h"
#include "alfabeta.h"
#include <stdio.h>

const u32 fla[256] __attribute__ ((section (".text\n\t#"), aligned(1024)));

#define SAVE(v) value = (u32*)&v; \
                flash_program_word((u32)&fla[i++], *value);
#define RESTORE(v, type, tmp) tmp = (type*)&fla[i++]; \
                              v     = *tmp;

u8 flash_load() {
	int i=0;
	float *tmp_f;
	u16 *tmp_u16;
	u32 *tmp_u32;
	u32 key;

	RESTORE(key, u32, tmp_u32);
	if(key != 0xA55A5AA5) {
		pid[0].P = 2;
		pid[1].P = 2;
		pid[2].P = 3;
		pid[3].P = 3;
		return 0;
	}

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

	RESTORE(pid[2].P, float, tmp_f);
	RESTORE(pid[2].I, float, tmp_f);
	RESTORE(pid[2].D, float, tmp_f);
	RESTORE(pid[2].I_sum_min, float, tmp_f);
	RESTORE(pid[2].I_sum_max, float, tmp_f);

	RESTORE(pid[3].P, float, tmp_f);
	RESTORE(pid[3].I, float, tmp_f);
	RESTORE(pid[3].D, float, tmp_f);
	RESTORE(pid[3].I_sum_min, float, tmp_f);
	RESTORE(pid[3].I_sum_max, float, tmp_f);

	RESTORE(estabilizador[0].offset, u16, tmp_u16);
	RESTORE(estabilizador[0].ganancia, float, tmp_f);

	RESTORE(estabilizador[1].offset, u16, tmp_u16);
	RESTORE(estabilizador[1].ganancia, float, tmp_f);

	RESTORE(alfabeta[0].P, u16, tmp_u16);
	RESTORE(alfabeta[0].I, u16, tmp_u16);
	RESTORE(alfabeta[1].P, u16, tmp_u16);
	RESTORE(alfabeta[1].I, u16, tmp_u16);

	RESTORE(gyroscope_gain[0], float, tmp_f);
	RESTORE(gyroscope_gain[1], float, tmp_f);
	RESTORE(gyroscope_gain[2], float, tmp_f);
	return 1;
}

#define SAVE(v) value = (u32*)&v; \
                flash_program_word((u32)&fla[i++], *value);


void flash_save() {
	int i=0;
	u32 *value;
	u32 key;

	printf("Salvando..\r\n");

	flash_unlock();
	flash_erase_page((u32)&fla[0]);

	key = 0xA55A5AA5;
	SAVE(key);

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

	SAVE(pid[2].P);
	SAVE(pid[2].I);
	SAVE(pid[2].D);
	SAVE(pid[2].I_sum_min);
	SAVE(pid[2].I_sum_max);

	SAVE(pid[3].P);
	SAVE(pid[3].I);
	SAVE(pid[3].D);
	SAVE(pid[3].I_sum_min);
	SAVE(pid[3].I_sum_max);

	SAVE(estabilizador[0].offset);
	SAVE(estabilizador[0].ganancia);

	SAVE(estabilizador[1].offset);
	SAVE(estabilizador[1].ganancia);

	SAVE(alfabeta[0].P);
	SAVE(alfabeta[0].I);
	SAVE(alfabeta[1].P);
	SAVE(alfabeta[1].I);

	SAVE(gyroscope_gain[0]);
	SAVE(gyroscope_gain[1]);
	SAVE(gyroscope_gain[2]);

	flash_lock();

	printf("Salvado!\r\n");
}

