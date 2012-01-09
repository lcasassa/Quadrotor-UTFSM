#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <libopencm3/cm3/common.h>

typedef s32 ring_size_t;

struct ring {
	u8 *data;
	ring_size_t size;
	u32 begin;
	u32 end;
};

#define RING_SIZE(RING)  ((RING)->size - 1)
#define RING_DATA(RING)  (RING)->data
#define RING_EMPTY(RING) ((RING)->begin == (RING)->end)

void ring_init(struct ring *ring, u8 *buf, ring_size_t size);
s32 ring_write_ch(struct ring *ring, u8 ch);
s32 ring_write(struct ring *ring, u8 *data, ring_size_t size);
s32 ring_read_ch(struct ring *ring, u8 *ch);
s32 ring_read(struct ring *ring, u8 *data, ring_size_t size);

#endif
