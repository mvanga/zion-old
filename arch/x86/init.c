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
	((uint16_t *)0xc00b8000)[0] = 'a' | (0xf0 << 8);
	for(;;);

	paging_init2();
	gdt_init();
//	do_initcalls_early();
	printk("Hello World\n");
	idt_init();

	/* we can safely enable interrupts now */
	asm volatile("sti");

	/* call the arch initcalls */
	do_initcalls_arch();

//	paging_init();

	return main();
}
