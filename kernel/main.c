#include <zion/stdio.h>
#include <zion/console.h>
#include <zion/types.h>
#include <zion/module.h>
#include <zion/jiffies.h>
#include <zion/bitset.h>
#include <zion/alloc.h>

int main(void)
{
	do_initcalls_core();
	int *a = kmalloc(sizeof(int), ALLOC_KERN | ALLOC_ALIGN);
	*a = 5;
	printk("%p %d\n", a, *a);
	kfree(a);
	a = kmalloc(sizeof(int), ALLOC_KERN);
	*a = 10;
	printk("%p %d\n", a, *a);
	while (1) {
		printk(".");
		delay(100);
	}
	do_exitcalls_core();
	return 0;
}
