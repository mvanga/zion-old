#include <zion/stdio.h>
#include <zion/console.h>

int main(void)
{
	printk("Hello World!\n");
	printk("Hello World!\n");
	asm volatile ("int $0x3");
	asm volatile ("int $0x4");
	printk("Hello World!\n");
	asm volatile ("int $0x3");
	for(;;);
	return 0;
}
