#ifndef ZION_ARCH_X86_ASM_PAGING_H
#define ZION_ARCH_X86_ASM_PAGING_H

#include <zion/types.h>

#define PAGE_FRAME_ADDR_MASK	0xfffff000
#define PAGE_UNUSED_MASK	0x00000fe0
#define PAGE_PRESENT		(1 << 0)
#define PAGE_WRITABLE		(1 << 1)
#define PAGE_USER		(1 << 2)

struct page {
	/* the frame associated with this page */
	uint32_t frame;
};

#define PTABLE_PRESENT	(1 << 0)
#define PTABLE_WRITABLE	(1 << 1)
#define PTABLE_USER	(1 << 2)

struct page_table {
	struct page pages[1024];
};

struct page_dir {
	struct page_table *tables[1024];
	uint32_t tables_phys[1024];
	uint32_t dir_phys_addr;
	uint32_t tables_phys_addr;
};

void paging_init_pre(void);
int paging_init(void);
void paging_pre_cleanup(void);

#endif
