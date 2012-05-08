#include <zion/stdio.h>
#include <zion/console.h>
#include <zion/types.h>
#include <zion/module.h>
#include <zion/jiffies.h>
#include <zion/bitset.h>
#include <zion/alloc.h>

#include <asm/task.h>

int main(void)
{
	char ch = 'B';
	int ret;

	do_initcalls_core();
	ret = fork();
	if (ret == 0)
		ch = 'B';
	while (1) {
		printk("%c", ch);
		delay(100);
	}
	do_exitcalls_core();
	return 0;
}
