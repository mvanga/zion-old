#include <asm/heap.h>
#include <zion/types.h>
#include <zion/stdlib.h>
#include <asm/paging.h>
#include <asm/mm.h>
#include <zion/stdio.h>

int heap_init(void)
{
	uint32_t i;
	uint32_t curr;

	curr = alloc_current;
	if (curr & 0xfffff000) {
		curr &= 0xfffff000;
		curr += 4096;
	}

	for (i = curr; i < KHEAP_END; i += 4096)
		page_frame_alloc(page_get(&kern_page_dir, i, 0), 1, 1);

	printk("mapped pages -> frames for heap from %p to %08x\n", curr, KHEAP_END);

	/* create_heap(curr, KHEAP_END, KHEAP_MAX, super=0, writable=1) */

	return 0;
}
