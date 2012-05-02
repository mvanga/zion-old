#include <zion/console.h>
#include <zion/io.h>
#include <zion/module.h>

#include <asm/segment.h>

extern int main();
extern struct console early_printk;

int arch_main(void)
{
	/* call early initcalls */
	do_initcalls_early();

	gdt_init();
	idt_init();

	/* we can safely enable interrupts now */
	asm volatile("sti");

	/* call the arch initcalls */
	do_initcalls_arch();

	return main();
}
