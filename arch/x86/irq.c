#include <zion/types.h>
#include <zion/stdio.h>

#include <asm/regs.h>

void isr_handler(struct regs regs)
{
	printk("caught exception: 0x%X\n", regs.int_no);
}
