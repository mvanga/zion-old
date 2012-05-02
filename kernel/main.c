#include <zion/stdio.h>
#include <zion/console.h>
#include <zion/types.h>
#include <zion/module.h>
#include <zion/jiffies.h>

int main(void)
{
	do_initcalls_core();
	int i = 0;
	while (1) {
		printk("tick = %d\n", i++);
		delay(100);
	}
	do_exitcalls_core();
	return 0;
}
