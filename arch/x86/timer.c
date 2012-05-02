#include <zion/module.h>
#include <zion/types.h>
#include <zion/jiffies.h>
#include <zion/stdio.h>

#include <asm/irq.h>
#include <asm/regs.h>
#include <asm/io.h>

static void timer_handler(struct regs *regs __attribute__((unused)))
{
	jiffies++;
}

int timer_init(void)
{
	uint32_t divisor;

	printk("init timer to %d Hz\n", HZ);
	request_irq(0, &timer_handler);

	divisor = 1193180 / HZ;
	outb(0x36, 0x43);
	outb(divisor & 0xff, 0x40);
	outb((divisor >> 8) & 0xff, 0x40);

	return 0;
}

void timer_exit(void)
{
}

arch_init(timer_init)
arch_exit(timer_exit)
