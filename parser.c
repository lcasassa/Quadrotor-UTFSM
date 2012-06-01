#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "parser.h"
#include "main.h"
#include "ringbuffer.h"
#include "usart.h"
#include "flash_.h"
#include "pid.h"
#include "estabilizador.h"
#include "alfabeta.h"
#include "ITG3200.h"
#include "ADXL345.h"

u8 parser_flag = 0;

void parser_check() {
	static char buffer[256]="";

	if(!parser_flag) return;

	while(parser_flag) {
		unsigned int checksum;
		unsigned int value;
		unsigned int index;
		s32 size;

		parser_flag--;

		if((size = ring_line_size(&input_ring)) == -1) return;
		if(size > 255) size = 255;

		if(size != ring_read(&input_ring, (u8 *)buffer, size)) return;
		buffer[size] = 0;
//printf("Hay Trama! size:%d buffer:%s \r\n", (int)size, buffer);

		if(size < 6) { printf("size<6\r\n"); return; } // $XP0*FD\r\n
		if(buffer[0] != '$') { printf("buffer[0] != '$'\r\n"); return; }
		if(buffer[size-4] != '*') { printf("buffer[size-4]=%c != '*'\r\n", buffer[size-4]); return; }

//		sscanf(buffer+size-2-2, "%X", &checksum); // 2 de checksum + "\n" = 3 caracteres
		checksum = strtoul(buffer+size-2-1, NULL, 16);


		
//printf("checksum = %X s:%s\r\n", checksum, buffer+size-2-2);
		if(checksum != parser_calculateChecksum(buffer+1, size-1-2-1-1)) { // 2 caracteres de "\n" + 2 caracteres por el checksum + 1 caracter por el asterisco + 1 porque el 0 cuenta.
			printf("Checksum ! %X != %X\r\n", (int)parser_calculateChecksum(buffer+1, size-2-2-1-1), checksum);
			return;
		}
//printf("procesando Trama\r\n");

		printf("#%s", &buffer[1]);

		switch(buffer[1]) {
			case 'E':
			index = strtoul(buffer+2, NULL, 10);
			if(buffer[2] == '1')
				index = 0;
			else if(buffer[2] == '2')
				index = 1;
			else {
				printf("error 1\r\n");
				break; 
			}
			switch(buffer[3]) {
				case 'O':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+4, NULL, 10);
				estabilizador[index].offset = value;
//				printf("estabilizador offset index:%d value:%d\r\n", index, value);
				break;
				case 'G':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+4, NULL, 10);
				estabilizador[index].ganancia = (float)value/1000;
//				printf("estabilizador ganancia index:%d value:%d\r\n", index, value);
				break;
			}
			break;
			case 'X':
			switch(buffer[2]) {
				case 'P':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+3, NULL, 10);
				pid[2].P = ((float)value)/1000.0;
				break;
				case 'I':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+3, NULL, 10);
				pid[2].I = ((float)value)/1000.0;
				break;
				case 'G':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+3, NULL, 10);
				pid[0].P = ((float)value)/1000.0;
				break;
				case 'A':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+3, NULL, 10);
				pid[0].I = ((float)value)/1000.0;
				break;
				case 'O':
				value = strtoul(buffer+3, NULL, 10);
				alfabeta[0].P = ((float)value)/1000.0;
				break;
				case 'L':
				value = strtoul(buffer+3, NULL, 10);
				alfabeta[0].I = ((float)value)/1000.0;
				break;
				case 'E':
				value = strtoul(buffer+3, NULL, 10);
				gyroscope_gain[0] = ((float)value)/1000.0;
				break;
				case 'F':
				value = strtoul(buffer+3, NULL, 10);
				accelerometer_offset[0] = value;
				break;
			}
			break;
			case 'Y':
			switch(buffer[2]) {
				case 'P':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+3, NULL, 10);
				pid[3].P = ((float)value)/1000.0;
				break;
				case 'I':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+3, NULL, 10);
				pid[3].I = ((float)value)/1000.0;
				break;
				case 'G':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+3, NULL, 10);
				pid[1].P = ((float)value)/1000.0;
				break;
				case 'A':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+3, NULL, 10);
				pid[1].I = ((float)value)/1000.0;
				break;
				case 'O':
				value = strtoul(buffer+3, NULL, 10);
				alfabeta[1].P = ((float)value)/1000.0;
				break;
				case 'L':
				value = strtoul(buffer+3, NULL, 10);
				alfabeta[1].I = ((float)value)/1000.0;
				break;
				case 'E':
				value = strtoul(buffer+3, NULL, 10);
				gyroscope_gain[1] = ((float)value)/1000.0;
				break;
				case 'F':
				value = strtoul(buffer+3, NULL, 10);
				accelerometer_offset[1] = value;
				break;
			}
			break;
			case 'Z':
			switch(buffer[2]) {
				case 'P':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+3, NULL, 10);
				break;
				case 'I':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+3, NULL, 10);
				break;
				case 'D':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+3, NULL, 10);
				break;
				case 'G':
				//sscanf(buffer+3,"%d", &value);
				value = strtoul(buffer+3, NULL, 10);
//				pid[0].I = ((float)value)/1000.0;
				break;
				case 'E':
				value = strtoul(buffer+3, NULL, 10);
				gyroscope_gain[2] = ((float)value)/1000.0;
				break;
				case 'F':
				value = strtoul(buffer+3, NULL, 10);
				accelerometer_offset[2] = value;
				break;
			}
			break;
			case 'W':
				flash_save();
				break;
			case 'G':
				printf("$XP%d*%02d\n", (int)(pid[2].P * 1000), 10);
				printf("$XI%d*%02d\n", (int)(pid[2].I * 1000), 10);
				printf("$XG%d*%02d\n", (int)(pid[0].P * 1000), 10);
				printf("$XA%d*%02d\n", (int)(pid[0].I * 1000), 10);
				printf("$YP%d*%02d\n", (int)(pid[3].P * 1000), 10);
				printf("$YI%d*%02d\n", (int)(pid[3].I * 1000), 10);
				printf("$YG%d*%02d\n", (int)(pid[1].P * 1000), 10);
				printf("$YA%d*%02d\n", (int)(pid[1].I * 1000), 10);
				printf("$XE%d*%02d\n", (int)(gyroscope_gain[0] * 1000), 10);
				printf("$YE%d*%02d\n", (int)(gyroscope_gain[1] * 1000), 10);
				printf("$ZE%d*%02d\n", (int)(gyroscope_gain[2] * 1000), 10);
				printf("$XO%d*%02d\n", (int)(alfabeta[0].P * 1000), 10);
				printf("$XL%d*%02d\n", (int)(alfabeta[0].I * 1000), 10);
				printf("$YO%d*%02d\n", (int)(alfabeta[1].P * 1000), 10);
				printf("$YL%d*%02d\n", (int)(alfabeta[1].I * 1000), 10);
				printf("$XF%d*%02d\n", (int)(accelerometer_offset[0]), 10);
				printf("$YF%d*%02d\n", (int)(accelerometer_offset[1]), 10);
				printf("$ZF%d*%02d\n", (int)(accelerometer_offset[2]), 10);
				break;
			case 'O':
				value = strtoul(buffer+2, NULL, 10);
				output = (int)value;
				break;
		}


	}

	return;
}

u8 parser_calculateChecksum(char* command, u16 length) {
	volatile u8 xor = 0;
	u16 i;
	for(i = 0; i < length; i++)
		xor ^= (u8)command[i];
	return xor;
}

