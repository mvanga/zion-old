#ifndef ZION_ALLOC_H
#define ZION_ALLOC_H

struct heap {
	struct alloc_area *alloc;
	uint32_t start;
	uint32_t brk;
	uint32_t max;
};

void kernel_heap_init(uint32_t, uint32_t, uint32_t);
void *kmalloc(uint32_t);
void kfree(void *);

#endif
