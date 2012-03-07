//#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/nvic.h>
#include <stdio.h>
#include <errno.h>

#include "usart.h"
#include "parser.h"

#define BUFFER_SIZE 1024

struct ring output_ring;
u8 output_ring_buffer[BUFFER_SIZE];
struct ring input_ring;
u8 input_ring_buffer[BUFFER_SIZE];

void usart_setup(void)
{
	/* Initialize output and input ring buffer. */
	ring_init(&output_ring, output_ring_buffer, BUFFER_SIZE);
	ring_init( &input_ring,  input_ring_buffer, BUFFER_SIZE);

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
	//usart_disable(USART1);
}

void usart1_isr(void)
{
//asm("push    {r0,r1,r2,r3}");
	/* Check if we were called because of RXNE. */
	if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {
		char c;
		c = usart_recv(USART1);
		ring_write_ch(&input_ring, c);
		if(c == '\n') 
			parser_flag++;
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
//asm("pop    {r0,r1,r2,r3}");
}

int _write(int file, char *ptr, int len)
{
	int ret;

	if (file == 1) {
		ret = ring_write(&output_ring, (u8 *)ptr, len);

		if (ret < 0)
			ret = -ret;

		USART_CR1(USART1) |= USART_CR1_TXEIE;

		return ret;
	}

	errno = EIO;
	return -1;
}

