#include <zion/module.h>
#include <zion/types.h>
#include <zion/stdio.h>

extern uint32_t __alloc_start_phys;
extern uint32_t __alloc_start;
uint32_t alloc_current_phys;
uint32_t alloc_current;

void *arch_kmalloc_internal(uint32_t size, int align, uint32_t *phys)
{
	uint32_t old;

	if (align && (alloc_current & 0xfffff000)) {
		alloc_current &= 0xfffff000;
		alloc_current += 4096;
		alloc_current_phys &= 0xfffff000;
		alloc_current_phys += 4096;
	}

	if (phys)
		*phys = alloc_current_phys;

	old = alloc_current;
	alloc_current += size;
	alloc_current_phys += size;

	return (void *)old;
}

void *arch_kmalloc_a(uint32_t size)
{
	return arch_kmalloc_internal(size, 1, 0);
}

void *arch_kmalloc_p(uint32_t size, uint32_t *phys)
{
	return arch_kmalloc_internal(size, 0, phys);
}

void *arch_kmalloc_ap(uint32_t size, uint32_t *phys)
{
	return arch_kmalloc_internal(size, 1, phys);
}

void *arch_kmalloc(uint32_t size)
{
	return arch_kmalloc_internal(size, 0, 0);
}

void arch_kfree(void *ptr __attribute__((unused)))
{
}


int mm_init(void)
{
	alloc_current = (uint32_t)&__alloc_start;
	alloc_current_phys = (uint32_t)&__alloc_start_phys;

	return 0;
}
