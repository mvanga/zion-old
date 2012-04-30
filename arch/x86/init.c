#include <zion/console.h>
#include <zion/io.h>

extern int main();
extern struct console early_printk;

int arch_main(void)
{
	int ret;

	ret = console_register(&early_printk);
	if (ret < 0)
		return -1;

	return main();
}
