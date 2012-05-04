#include <zion/console.h>
#include <zion/io.h>
#include <asm/alloc.h>
#include <zion/module.h>
#include <zion/stdio.h>

#include <asm/mm.h>
#include <asm/segment.h>
#include <asm/paging.h>

extern int main();
extern struct console early_printk;

int arch_main(void)
{
	/* call early initcalls */
	do_initcalls_early();

	/* map the first 4mb (phys) to 3gb (virtual) */
	paging_init_pre();
	/* get rid of trickgdt and put a proper one */
	gdt_init();
	mm_init();
	paging_init();
	/* now we can setup interrupts */
	idt_init();
	/* unmap the first 4mb of pages */
	paging_init_post();

	/* we can safely enable interrupts here */
	asm volatile("sti");

	/* call the arch initcalls */
	do_initcalls_arch();

	printk("alloc_current = %p\n", alloc_current);


	return main();
}
