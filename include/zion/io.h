#ifndef ZION_IO_H
#define ZION_IO_H

#include <asm/io.h>

static inline uint16_t readw(uint32_t addr)
{
	return ((uint16_t *)addr)[0];
}

static inline void writew(uint16_t val, uint32_t addr)
{
	((uint16_t *)addr)[0] = val;
}

#endif
