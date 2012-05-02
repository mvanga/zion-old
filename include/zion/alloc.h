#ifndef ZION_MM_H
#define ZION_MM_H

#include <zion/types.h>

extern uint32_t alloc_current;

void *kmalloc_internal(uint32_t size, int align, uint32_t *phys);
void *kmalloc_a(uint32_t size);
void *kmalloc_p(uint32_t size, uint32_t *phys);
void *kmalloc_ap(uint32_t size, uint32_t *phys);
void *kmalloc(uint32_t size);
void kfree(void *);

#endif
