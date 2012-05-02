#include <zion/stdio.h>
#include <zion/console.h>
#include <zion/types.h>
#include <zion/module.h>
#include <zion/jiffies.h>
#include <zion/bitset.h>

int main(void)
{
	do_initcalls_core();
	while (1) {
		printk(".");
		delay(100);
	}
	do_exitcalls_core();
	return 0;
}
