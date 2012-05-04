#include <zion/console.h>
#include <zion/io.h>
#include <zion/module.h>

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
	/* unmap the first 4mb of pages */
	paging_pre_cleanup();

	/* now we can happily setup interrupts */
	idt_init();

	/* we can safely enable interrupts here */
	asm volatile("sti");

	/* call the arch initcalls */
	do_initcalls_arch();

//	paging_init();

	return main();
}
