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
	paging_init2();
	gdt_init();
	paging_cleanup();
	do_initcalls_early();
	printk("Hello Higher Half World: %p\n", &arch_main);
	idt_init();

	/* we can safely enable interrupts now */
	asm volatile("sti");

	/* call the arch initcalls */
	do_initcalls_arch();

//	paging_init();

	return main();
}
