//#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/nvic.h>
#include <stdio.h>
#include <errno.h>

#include "ringbuffer.h"
#include "usart.h"
#include "flash_.h"
#include "pid.h"

#define BUFFER_SIZE 1024

struct ring output_ring;
u8 output_ring_buffer[BUFFER_SIZE];

void usart_setup(void)
{
	/* Initialize output ring buffer. */
	ring_init(&output_ring, output_ring_buffer, BUFFER_SIZE);

	/* Enable the USART1 interrupt. */
	nvic_enable_irq(NVIC_USART1_IRQ);

	/* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port B for transmit. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

	/* Setup GPIO pin GPIO_USART1_RE_RX on GPIO port B for receive. */
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
		      GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

	/* Setup UART parameters. */
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART1, USART_MODE_TX_RX);

	/* Enable USART1 Receive interrupt. */
	USART_CR1(USART1) |= USART_CR1_RXNEIE;

	/* Finally enable the USART. */
	usart_enable(USART1);
}

void usart1_isr(void)
{
	/* Check if we were called because of RXNE. */
	if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {
		char c;

		/* Indicate that we got data. */
		gpio_toggle(GPIOC, GPIO12);
/*
		// Retrieve the data from the peripheral.
		if(c == 's') {
			flash_save();
		} else if(c == 'l') {
			flash_load();
			printf("load: %f\r\n", pid[0].P);
		} else if(c == 'c') {
			printf("%f\r\n", ++pid[0].P);
		}

		// Enable transmit interrupt so it sends back the data.
*/
		c = usart_recv(USART1);
//		ring_write_ch(&output_ring, c);
//		USART_CR1(USART1) |= USART_CR1_TXEIE;
//printf("%c", c);
		usart_new_char(c);
	}

	/* Check if we were called because of TXE. */
	if (((USART_CR1(USART1) & USART_CR1_TXEIE) != 0) &&
	    ((USART_SR(USART1) & USART_SR_TXE) != 0)) {

		s32 data;

		data = ring_read_ch(&output_ring, NULL);

		if (data == -1) {
			/* Disable the TXE interrupt, it's no longer needed. */
			USART_CR1(USART1) &= ~USART_CR1_TXEIE;
		} else {
			/* Put data into the transmit register. */
			usart_send(USART1, data);
		}
	}
}

int _write(int file, char *ptr, int len)
{
	int ret,i;

	if (file == 1) {
for(i=0; i<len;i++)
usart_send_blocking(USART1, ptr[i]);
/*
		ret = ring_write(&output_ring, (u8 *)ptr, len);

		if (ret < 0)
			ret = -ret;

		USART_CR1(USART1) |= USART_CR1_TXEIE;
*/
		return i;//ret;
	}

	errno = EIO;
	return -1;
}

void usart_new_char(char c) {
	static char buffer[256]="";
	static uint8_t i = 0;
	int j;
	//static uint8_t count=0;
	int value = 0;
	buffer[i] = c;
	
	if(buffer[i] == '$') { // Start Char
		i=0;
		buffer[i++] = '$';
		return;
	}
		
	if(buffer[i++] == '\n') {
		unsigned int checksum;
		buffer[i] = 0;

		sscanf(buffer+i-2-2, "%02X", &checksum); // 2 de checksum + "\r\n" = 4 caracteres
		if(checksum != usart_calculateChecksum(buffer+1, i-2-2-1-1)) { // 2 caracteres de "\r\n" + 2 caracteres por el checksum + 1 caracter por el asterisco + 1 porque el 0 cuenta.
			//printf("Checksum ! %d\r\n", (int)usart_calculateChecksum(buffer+1, i-2-2-1-1));
			i=0;
			return;
		}
		if(i<3) {
			//printf("i<3!\r\n");
			i=0;
			return;
		}
 		i=0;
		
		if(buffer[0] != '$') {
			//printf("buffer[0] != '$'\r\n");
			return;
		}			
		switch(buffer[1]) {
			case 'X':
			switch(buffer[2]) {
				case 'P':
				sscanf(buffer+3,"%d", &value);
				pid[0].P = ((float)value)/1000.0;
				break;
				case 'I':
				sscanf(buffer+3,"%d", &value);
				pid[0].D_ref = ((float)value)/1000.0;
				break;
				case 'D':
				sscanf(buffer+3,"%d", &value);
				pid[0].D_med = ((float)value)/1000.0;
				break;
			}
			break;
			case 'Y':
			switch(buffer[2]) {
				case 'P':
				sscanf(buffer+3,"%d", &value);
				pid[1].P = ((float)value)/1000.0;
				break;
				case 'I':
				sscanf(buffer+3,"%d", &value);
				pid[1].D_ref = ((float)value)/1000.0;
				break;
				case 'D':
				sscanf(buffer+3,"%d", &value);
				pid[1].D_med = ((float)value)/1000.0;
				break;
			}
			break;
			case 'Z':
			switch(buffer[2]) {
				case 'P':
				sscanf(buffer+3,"%d", &value);
				break;
				case 'I':
				sscanf(buffer+3,"%d", &value);
				break;
				case 'D':
				sscanf(buffer+3,"%d", &value);
				break;
			}
			break;
			case 'W':
				flash_save();
				break;
			case 'G':
				j=0;
				j += sprintf(&buffer[i+7+j], "$XP%d*%02d\n", (int)(pid[0].P     * 1000), 10);
				j += sprintf(&buffer[i+7+j], "$XI%d*%02d\n", (int)(pid[0].D_ref * 1000), 10);
				j += sprintf(&buffer[i+7+j], "$XD%d*%02d\n", (int)(pid[0].D_med * 1000), 10);
				j += sprintf(&buffer[i+7+j], "$YP%d*%02d\n", (int)(pid[1].P     * 1000), 10);
				j += sprintf(&buffer[i+7+j], "$YI%d*%02d\n", (int)(pid[1].D_ref * 1000), 10);
				j += sprintf(&buffer[i+7+j], "$YD%d*%02d\n", (int)(pid[1].D_med * 1000), 10);
				break;
		}

		//printf("%s", buffer);
		ring_write_ch(&output_ring, '\n');
		for(j=0; buffer[j] != 0; j++) {
			ring_write_ch(&output_ring, buffer[j]);
		}
		USART_CR1(USART1) |= USART_CR1_TXEIE;

	}
}

unsigned char usart_calculateChecksum(char* command, int length) {
	volatile unsigned char xor = 0;
	int i;
	for(i = 0; i < length; i++)
		xor ^= (unsigned char)command[i];
	return xor;
}

