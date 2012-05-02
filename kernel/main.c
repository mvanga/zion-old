#include <zion/stdio.h>
#include <zion/console.h>
#include <zion/types.h>
#include <zion/module.h>
#include <zion/jiffies.h>

extern init_t __init_begin;
extern init_t __init_end;
extern exit_t __exit_begin;
extern exit_t __exit_end;

void do_initcalls(void)
{
	init_t *call;
	call = &__init_begin;
	do {
		(*call)();
		call++;
	} while (call < &__init_end);
}

void do_exitcalls(void)
{
	exit_t *call;
	/* Deinitialize in reverse */
	call = &__exit_end;
	call--;
	do {
		(*call)();
		call--;
	} while (call >= &__exit_begin);
}


int main(void)
{
	do_initcalls();
	int i = 0;
	while (1) {
		printk("tick = %d\n", i++);
		delay(100);
	}
	do_exitcalls();
	return 0;
}
