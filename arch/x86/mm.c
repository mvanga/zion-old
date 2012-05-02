#include <zion/module.h>
#include <zion/types.h>
#include <zion/stdio.h>

extern uint32_t __alloc_start;
uint32_t alloc_current;

void *kmalloc_internal(uint32_t size, int align, uint32_t *phys)
{
	uint32_t old;

	if (align && (alloc_current & 0xfffff000)) {
		alloc_current &= 0xfffff000;
		alloc_current += 4096;
	}

	if (phys)
		*phys = alloc_current;

	old = alloc_current;
	alloc_current += size;

	return (void *)old;
}

void *kmalloc_a(uint32_t size)
{
	return kmalloc_internal(size, 1, 0);
}

void *kmalloc_p(uint32_t size, uint32_t *phys)
{
	return kmalloc_internal(size, 0, phys);
}

void *kmalloc_ap(uint32_t size, uint32_t *phys)
{
	return kmalloc_internal(size, 1, phys);
}

void *kmalloc(uint32_t size)
{
	return kmalloc_internal(size, 0, 0);
}

void kfree(void *ptr __attribute__((unused)))
{
}


int mm_arch_init(void)
{
	alloc_current = (uint32_t)&__alloc_start;

	return 0;
}

arch_init(mm_arch_init)
