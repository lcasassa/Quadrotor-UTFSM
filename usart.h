#ifndef USART_H
#define USART_H

#include "ringbuffer.h"

//#define printf(...) ;
//#define USART_BLOCKING 1

void usart_setup(void);
void usart_new_char(char c);
unsigned char usart_calculateChecksum(char* command, int length);
void uart_write_block(char*ptr, int len);

extern struct ring output_ring;
extern struct ring input_ring;
#endif
