/******************************************************************************
 * Simple ringbuffer implementation from open-bldc's libgovernor that
 * you can find at:
 * https://github.com/open-bldc/open-bldc/tree/master/source/libgovernor
 *****************************************************************************/

#include "ringbuffer.h"
#include <stdio.h>
#include <libopencm3/stm32/usart.h>

void ring_init(struct ring *ring, u8 *buf, ring_size_t size)
{
	ring->data = buf;
	ring->size = size;
	ring->begin = 0;
	ring->end = 0;
}

s32 ring_write_ch(struct ring *ring, u8 ch)
{
	USART_CR1(USART1) &= ~USART_CR1_RXNEIE;
	USART_CR1(USART1) &= ~USART_CR1_TXEIE;

	if (((ring->end + 1) % ring->size) != ring->begin) {
		ring->data[ring->end++] = ch;
		ring->end %= ring->size;

		USART_CR1(USART1) |= USART_CR1_RXNEIE;
		USART_CR1(USART1) |= USART_CR1_TXEIE;

		return (u32)ch;
	}

	USART_CR1(USART1) |= USART_CR1_RXNEIE;
	USART_CR1(USART1) |= USART_CR1_TXEIE;

	return -1;
}

s32 ring_write(struct ring *ring, u8 *data, ring_size_t size)
{
	s32 i;

	for (i = 0; i < size; i++) {
		if (ring_write_ch(ring, data[i]) < 0)
			return -i;
	}

	return i;
}

s32 ring_read_ch(struct ring *ring, u8 *ch)
{
	s32 ret = -1;
	USART_CR1(USART1) &= ~USART_CR1_RXNEIE;
	USART_CR1(USART1) &= ~USART_CR1_TXEIE;

	if (ring->begin != ring->end) {
		ret = ring->data[ring->begin++];
		ring->begin %= ring->size;
		if (ch)
			*ch = ret;
	}

	USART_CR1(USART1) |= USART_CR1_RXNEIE;
	USART_CR1(USART1) |= USART_CR1_TXEIE;
	return ret;
}

s32 ring_read(struct ring *ring, u8 *data, ring_size_t size)
{
	s32 i;

	for (i = 0; i < size; i++) {
		if (ring_read_ch(ring, data + i) < 0)
			return -i;
	}

	return i;
}

s32 ring_line_size(struct ring *ring)
{
	u32 i;
	u32 end = (ring->end < ring->begin) ? ring->end + ring->size : ring->end;

	for (i = ring->begin; i < end; i++) {
		if (ring->data[i%ring->size] == '\n')
			return i - ring->begin+1;
	}

	return -1;
}
