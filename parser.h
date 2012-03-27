#ifndef PARSER_H
#define PARSER_H

#include <libopencm3/cm3/common.h>

extern u8 parser_flag;

void parser_check();
u8 parser_calculateChecksum(char* command, u16 length);

#endif

