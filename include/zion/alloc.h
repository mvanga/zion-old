#ifndef ZION_ALLOC_H
#define ZION_ALLOC_H

#include <zion/types.h>

#define ALLOC_KERN	(1 << 0)
#define ALLOC_ALIGN	(1 << 1)

struct heap {
	uint32_t valid;
	struct alloc_area *alloc;
	uint32_t start;
	uint32_t brk;
	uint32_t max;
};

extern struct heap kern_heap;

void kernel_heap_init(uint32_t, uint32_t, uint32_t);
void *kmalloc(uint32_t, uint32_t);
void kfree(void *);

#endif
