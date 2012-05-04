#include <zion/types.h>
#include <zion/bitset.h>
#include <zion/stdio.h>
#include <zion/stdlib.h>
#include <zion/string.h>

#include <asm/irq.h>
#include <asm/paging.h>
#include <asm/alloc.h>

struct bitset *frames;

/* The current page directory */
struct page_dir *curr_page_dir;
/* The page directory for the kernel */
struct page_dir kern_page_dir __attribute__ ((aligned (4096)));
struct page_table page_table_low __attribute__ ((aligned (4096)));

extern uint32_t alloc_current;

struct bitset *arch_bitset_create(int nbits)
{
	int len;
	struct bitset *b;

	len = (nbits/(sizeof(uint32_t)*8)) +
		((nbits%(sizeof(uint32_t)*8)) ? 1 : 0);

	b = arch_kmalloc(sizeof(struct bitset));
	if (!b)
		return NULL;
	b->bits = arch_kmalloc(len * sizeof(uint32_t));
	/* ignore alloc failures */
	if (!b->bits)
		return NULL;

	memset(b->bits, 0, len*sizeof(uint32_t));
	b->len = len;

	return b;
}


/* looks for an empty frame for this page */
int page_frame_alloc(struct page *p, int user, int write)
{
	int i;
	if (!p)
		return -1;
	if (p->frame & PAGE_FRAME_ADDR_MASK)
		return -1;

	i = bit_find_first_free(frames);
	if (i < 0) {
		printk("No free frames left!\n");
		for (;;);
	}
	bit_set(frames, i);
	p->frame = 0;
	p->frame |= PAGE_PRESENT;
	if (write)
		p->frame |= PAGE_WRITABLE;
	if (user)
		p->frame |= PAGE_USER;
	p->frame |= ((i << 12) & PAGE_FRAME_ADDR_MASK);

	return 0;
}

/* free the frame associated with this page */
void page_frame_free(struct page *page)
{
	if (!(page->frame & PAGE_FRAME_ADDR_MASK))
		return;
	bit_clear(frames, page->frame >> 12);
	page->frame = 0;
}

struct page *page_get(struct page_dir *dir, uint32_t addr, int alloc)
{
	addr >>= 12;

	/* If the table doesn't exist alloc or return */
	if (!dir->tables[addr/1024]) {
		uint32_t phys;

		if (!alloc)
			return NULL;

		dir->tables[addr/1024] = arch_kmalloc_ap(sizeof(struct page_table), &phys);
		memset(dir->tables[addr/1024], 0, sizeof(struct page_table));
		dir->tables_phys[addr/1024] = phys | PTABLE_PRESENT |
			PTABLE_WRITABLE | PTABLE_USER;
	}
	return &dir->tables[addr/1024]->pages[addr%1024];
}

void switch_page_dir(struct page_dir *dir)
{
	uint32_t cr0;

	curr_page_dir = dir;
	asm volatile("mov %0, %%cr3":: "r"(dir->tables_phys_addr));
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging!
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void page_fault_handler(struct regs *r)
{
	uint32_t fault_addr;
	int present;
	int rw;
	int us;
	int reserved;
	int id;

	/*
	 * A page fault has occurred. The faulting address is stored in the CR2
	 * register.
	 */
	asm volatile("mov %%cr2, %0" : "=r" (fault_addr));

	/* The error code gives us details of what happened. */
	present   = !(r->err_code & 0x1);	/* Page not present */
	rw = r->err_code & 0x2;		/* Write operation? */
	us = r->err_code & 0x4;		/* Processor was in user-mode? */
	reserved = r->err_code & 0x8;		/* Overwritten CPU-reserved bits of page entry? */
	id = r->err_code & 0x10;		/* Caused by an instruction fetch? */

	printk("Page fault! ( ");
	if (present)
		printk("present ");
	if (rw)
		printk("read-only ");
	if (us)
		printk("user-mode ");
	if (reserved)
		printk("reserved ");
	if (id)
		printk("fetch ");
	printk(") @ 0x%08x\n", fault_addr);
	for (;;);
}

#if 0
int paging_init(void)
{
	uint32_t i = 0;
	uint32_t mem_end = 0x1000000;

	frames = arch_bitset_create(mem_end/4096);

	kern_page_dir = arch_kmalloc_a(sizeof(struct page_dir));
	memset(kern_page_dir, 0, sizeof(struct page_dir));

	while (i < alloc_current) {
		page_frame_alloc(page_get(kern_page_dir, i, 1), 0, 0);
		i += 4096;
	}

	request_exception(14, page_fault_handler);

	switch_page_dir(kern_page_dir);

	return 0;
}
#endif

/*
 * This function fills the page directory and the page table,
 * then enables paging by putting the address of the page directory
 * into the CR3 register and setting the 31st bit into the CR0 one
 */
void paging_init_pre(void)
{
	/* Pointers to the page directory and the page table */
	void *kpaged_ptr  = 0;
	void *kpaged_tbl_ptr = 0;
	void *lowpagetable_ptr = 0;
	int k = 0;

	/*
	 * Translate the page directory from virtual address to physical
	 * address. Also for the page table.
	 */
	kpaged_ptr = (char *)&kern_page_dir + 0x40000000;
	kpaged_tbl_ptr = (char *)&kern_page_dir.tables_phys + 0x40000000;
	lowpagetable_ptr = (char *)&page_table_low + 0x40000000;

	/* map lowest 4MB and clear page directory */
	kern_page_dir.dir_phys_addr = (uint32_t)kpaged_ptr;
	kern_page_dir.tables_phys_addr = (uint32_t)kpaged_tbl_ptr;
	for (k = 0; k < 1024; k++) {
		page_table_low.pages[k].frame = (k * 4096) | 0x3;
		kern_page_dir.tables[k] = 0;
		kern_page_dir.tables_phys[k] = 0;
	}

	/*
	 * Fills the addresses 0...4MB and 3072MB...3076MB of the page directory
	 * with the same page table
	 */
	kern_page_dir.tables_phys[0] = (uint32_t)lowpagetable_ptr | 0x3;
	kern_page_dir.tables_phys[768] = (uint32_t)lowpagetable_ptr | 0x3;
	kern_page_dir.tables[0] = &page_table_low;
	kern_page_dir.tables[768] = &page_table_low;

	/*
	 * Copies the address of the page directory into the CR3 register and,
	 * finally, enables paging!
	 */
	switch_page_dir(&kern_page_dir);
}

void paging_init_post(void)
{
	kern_page_dir.tables_phys[0] = 0;
	kern_page_dir.tables[0] = 0;
}
