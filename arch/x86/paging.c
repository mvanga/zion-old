#include <zion/types.h>
#include <zion/bitset.h>
#include <zion/stdio.h>
#include <zion/stdlib.h>
#include <zion/string.h>
#include <zion/alloc.h>

#include <asm/irq.h>
#include <asm/paging.h>
#include <asm/alloc.h>

struct bitset frames;

/* The current page directory */
struct page_dir *curr_page_dir;
/* The page directory for the kernel */
struct page_dir kern_page_dir __attribute__ ((aligned (4096)));
struct page_table page_table_low __attribute__ ((aligned (4096)));

extern uint32_t alloc_current;
extern void phys_frame_clone(uint32_t src, uint32_t dest);

extern uint32_t __stack_top;
extern uint32_t __stack_bottom;

void arch_bitset_create(struct bitset *b, int nbits)
{
	int len;

	len = (nbits/(sizeof(uint32_t)*8)) +
		((nbits%(sizeof(uint32_t)*8)) ? 1 : 0);

	b->bits = arch_kmalloc(len * sizeof(uint32_t));
	memset(b->bits, 0, len*sizeof(uint32_t));
	b->len = len;
}


/* looks for an empty frame for this page */
int page_frame_alloc(struct page *p, int user, int write)
{
	int i;
	if (!p)
		return -1;
	if (p->frame & PAGE_FRAME_ADDR_MASK)
		return -1;

	i = bit_find_first_free(&frames);
	if (i < 0) {
		printk("No free frames left!\n");
		for (;;);
	}
	bit_set(&frames, i);
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
	bit_clear(&frames, page->frame >> 12);
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

		dir->tables[addr/1024] = kmalloc(sizeof(struct page_table),
			ALLOC_KERN | ALLOC_ALIGN);
		phys = virt_to_phys(dir->tables[addr/1024]);
		memset(dir->tables[addr/1024], 0, sizeof(struct page_table));
		dir->tables_phys[addr/1024] = phys | PTABLE_PRESENT |
			PTABLE_WRITABLE | PTABLE_USER;
	} else {
		uint32_t frame_idx;

		frame_idx = dir->tables[addr/1024]->pages[addr%1024].frame >> 12;
		if (!bit_test(&frames, frame_idx))
			bit_set(&frames, frame_idx);
	}
	return &dir->tables[addr/1024]->pages[addr%1024];
}

void switch_page_dir(struct page_dir *dir)
{
	uint32_t cr0;

	curr_page_dir = dir;
	asm volatile("mov %0, %%cr3":: "r"(dir->tables_phys_addr));
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	/* Enable paging! */
	cr0 |= 0x80000000;
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
	rw = r->err_code & 0x2;			/* Write operation? */
	us = r->err_code & 0x4;			/* Processor was in user-mode? */
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

int paging_alloc_init(void)
{
	uint32_t i = 0;
	uint32_t mem_end = 0x1000000;

	arch_bitset_create(&frames, mem_end/4096);

	for (i = 0; i < 1024; i++)
		page_get(&kern_page_dir, i << 12, 0);

	return 0;
}

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
	request_exception(14, page_fault_handler);
}

void kern_page_dir_clone(void)
{
	curr_page_dir = page_dir_clone(&kern_page_dir);
	switch_page_dir(curr_page_dir);
}

uint32_t virt_to_phys(void *vaddr)
{
	struct page *p;
	
	if (kern_heap.valid) {
		p = page_get(curr_page_dir, (uint32_t)vaddr, 0);
		return (p->frame & PAGE_FRAME_ADDR_MASK) | ((uint32_t)vaddr & 0xfff);
	} else {
		return (uint32_t)vaddr - 0xc0000000;
	}
}

struct page_table *page_table_clone(struct page_table *src, uint32_t *phys)
{
	int i;
	struct page_table *t;

	t = kmalloc(sizeof(struct page_table), ALLOC_KERN | ALLOC_ALIGN);
	if (t) {
		printk("failed to clone page table\n");
		for (;;);
	}
	*phys = virt_to_phys(t);
	memset(t, 0, sizeof(struct page_table));

	for (i = 0; i < 1024; i++) {
		if (!src->pages[i].frame)
			continue;
		page_frame_alloc(&t->pages[i], 1, 0);
		t->pages[i].frame |= src->pages[i].frame & ~PAGE_FRAME_ADDR_MASK;
		phys_frame_clone(src->pages[i].frame << 12, t->pages[i].frame << 12);
	}
	return t;
}

extern uint32_t __stack_start;
extern uint32_t __stack_end;

struct page_dir *page_dir_clone(struct page_dir *src)
{
	int i;
	uint32_t phys;
	struct page_dir *new;

	new = kmalloc(sizeof(struct page_dir), ALLOC_KERN | ALLOC_ALIGN);
	if (!new) {
		printk("failed to clone directory\n");
		for (;;);
	}
	memset(new, 0, sizeof(struct page_dir));
	phys = virt_to_phys(&new->tables_phys);
	new->tables_phys_addr = phys;

	int stack_end = ((uint32_t)(&__stack_end) >> 12)/1024;
	int stack_start = ((uint32_t)(&__stack_start) >> 12)/1024;
	printk("stack: %d to %d\n", stack_start, stack_end);

	for (i = 0; i < 1024; i++) {
		if (!src->tables[i])
			continue;
		if (kern_page_dir.tables[i] == src->tables[i]) {
			new->tables[i] = src->tables[i];
			new->tables_phys[i] = src->tables_phys[i];
		} else {
			new->tables[i] = page_table_clone(src->tables[i], &phys);
			new->tables_phys[i] = phys | 0x7;
		}
	}
	return new;
}

extern uint32_t __stack_start;

void move_stack(void *new_stack_start, uint32_t size)
{
	uint32_t i;
	// Allocate some space for the new stack.
	for( i = (uint32_t)new_stack_start;
			i >= ((uint32_t)new_stack_start-size);
			i -= 0x1000)
	{
		// General-purpose stack is in user-mode.
		page_frame_alloc( page_get(curr_page_dir, i, 1), 1 /* User mode */, 1 /* Is writable */ );
	}

	// Flush the TLB by reading and writing the page directory address again.
	uint32_t pd_addr;
	asm volatile("mov %%cr3, %0" : "=r" (pd_addr));
	asm volatile("mov %0, %%cr3" : : "r" (pd_addr));

	// Old ESP and EBP, read from registers.
	uint32_t old_stack_pointer; asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
	uint32_t old_base_pointer;  asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer));

	// Offset to add to old stack addresses to get a new stack address.
	uint32_t offset            = (uint32_t)new_stack_start - __stack_start;

	// New ESP and EBP.
	uint32_t new_stack_pointer = old_stack_pointer + offset;
	uint32_t new_base_pointer  = old_base_pointer  + offset;

	// Copy the stack.
	memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, __stack_start-old_stack_pointer);

	// Backtrace through the original stack, copying new values into
	// the new stack.  
	for(i = (uint32_t)new_stack_start; i > (uint32_t)new_stack_start-size; i -= 4)
	{
		uint32_t tmp = * (uint32_t*)i;
		// If the value of tmp is inside the range of the old stack, assume it is a base pointer
		// and remap it. This will unfortunately remap ANY value in this range, whether they are
		// base pointers or not.
		if (( old_stack_pointer < tmp) && (tmp < __stack_start))
		{
			tmp = tmp + offset;
			uint32_t *tmp2 = (uint32_t*)i;
			*tmp2 = tmp;
		}
	}

	// Change stacks.
	asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
	asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer));
}
