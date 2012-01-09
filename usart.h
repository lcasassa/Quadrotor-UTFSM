#ifndef USART_H
#define USART_H

//#define printf(...) ;

void usart_setup(void);
void usart_new_char(char c);
unsigned char usart_calculateChecksum(char* command, int length);

#endif
