#include <libopencm3/stm32/f1/flash.h>
#include "flash_.h"
#include "pid.h"
#include <stdio.h>

const u32 fla[256] __attribute__ ((section (".text\n\t#"), aligned(1024)));

void flash_load() {
	int i=0;
	float *value;
	value = (float*)&fla[i++];

	pid[0].P     = *value;
	value = (float*)&fla[i++];
	pid[0].D_ref = *value;
	value = (float*)&fla[i++];
	pid[0].D_med = *value;
}

void flash_save() {
	int i=0;
	u32 *value;
printf("Salvando..\r\n");
	flash_unlock();
	flash_erase_page((u32)&fla[0]);
	value = (u32*)&pid[0].P;
	
	flash_program_word((u32)&fla[i++], *value);
	value = (u32*)&pid[0].D_ref;
	flash_program_word((u32)&fla[i++], *value);
	value = (u32*)&pid[0].D_med;
	flash_program_word((u32)&fla[i++], *value);
	flash_lock();
printf("Salvado!\r\n");
}

