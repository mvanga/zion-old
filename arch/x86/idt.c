#include <asm/segment.h>
#include <asm/io.h>
#include <asm/irq.h>

#include <zion/string.h>
#include <zion/stdio.h>

extern void idt_write(uint32_t);

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

struct idt_entry idt_table[256];
struct idt_ptr idt_ptr;

void debug(uint32_t a) { printk("0x%X 0x%X\n", a, &idt_ptr); for(;;); }

static void irq_remap(void)
{
	outb(0x11, 0x20);
	outb(0x11, 0xa0);
	outb(0x20, 0x21);
	outb(0x28, 0xa1);
	outb(0x04, 0x21);
	outb(0x02, 0xa1);
	outb(0x01, 0x21);
	outb(0x01, 0xa1);
	outb(0x0, 0x21);
	outb(0x0, 0xa1);
}

int idt_init(void)
{
	idt_ptr.limit = sizeof(struct idt_entry) * 256 -1;
	idt_ptr.base  = (uint32_t)&idt_table;

	memset(&idt_table, 0, sizeof(struct idt_entry) * 256);
	memset(&irq_handlers, 0, sizeof(isr_t) * 256);

	/* remap the irq's */
	irq_remap();

	idt_set(0, (uint32_t)isr0, 0x8, 0x8E);
	idt_set(1, (uint32_t)isr1, 0x8, 0x8E);
	idt_set(2, (uint32_t)isr2, 0x8, 0x8E);
	idt_set(3, (uint32_t)isr3, 0x8, 0x8E);
	idt_set(4, (uint32_t)isr4, 0x8, 0x8E);
	idt_set(5, (uint32_t)isr5, 0x8, 0x8E);
	idt_set(6, (uint32_t)isr6, 0x8, 0x8E);
	idt_set(7, (uint32_t)isr7, 0x8, 0x8E);
	idt_set(8, (uint32_t)isr8, 0x8, 0x8E);
	idt_set(9, (uint32_t)isr9, 0x8, 0x8E);
	idt_set(10, (uint32_t)isr10, 0x8, 0x8E);
	idt_set(11, (uint32_t)isr11, 0x8, 0x8E);
	idt_set(12, (uint32_t)isr12, 0x8, 0x8E);
	idt_set(13, (uint32_t)isr13, 0x8, 0x8E);
	idt_set(14, (uint32_t)isr14, 0x8, 0x8E);
	idt_set(15, (uint32_t)isr15, 0x8, 0x8E);
	idt_set(16, (uint32_t)isr16, 0x8, 0x8E);
	idt_set(17, (uint32_t)isr17, 0x8, 0x8E);
	idt_set(18, (uint32_t)isr18, 0x8, 0x8E);
	idt_set(19, (uint32_t)isr19, 0x8, 0x8E);
	idt_set(20, (uint32_t)isr20, 0x8, 0x8E);
	idt_set(21, (uint32_t)isr21, 0x8, 0x8E);
	idt_set(22, (uint32_t)isr22, 0x8, 0x8E);
	idt_set(23, (uint32_t)isr23, 0x8, 0x8E);
	idt_set(24, (uint32_t)isr24, 0x8, 0x8E);
	idt_set(25, (uint32_t)isr25, 0x8, 0x8E);
	idt_set(26, (uint32_t)isr26, 0x8, 0x8E);
	idt_set(27, (uint32_t)isr27, 0x8, 0x8E);
	idt_set(28, (uint32_t)isr28, 0x8, 0x8E);
	idt_set(29, (uint32_t)isr29, 0x8, 0x8E);
	idt_set(30, (uint32_t)isr30, 0x8, 0x8E);
	idt_set(31, (uint32_t)isr31, 0x8, 0x8E);

	idt_set(32, (uint32_t)irq0, 0x8, 0x8E);
	idt_set(33, (uint32_t)irq1, 0x8, 0x8E);
	idt_set(34, (uint32_t)irq2, 0x8, 0x8E);
	idt_set(35, (uint32_t)irq3, 0x8, 0x8E);
	idt_set(36, (uint32_t)irq4, 0x8, 0x8E);
	idt_set(37, (uint32_t)irq5, 0x8, 0x8E);
	idt_set(38, (uint32_t)irq6, 0x8, 0x8E);
	idt_set(39, (uint32_t)irq7, 0x8, 0x8E);
	idt_set(40, (uint32_t)irq8, 0x8, 0x8E);
	idt_set(41, (uint32_t)irq9, 0x8, 0x8E);
	idt_set(42, (uint32_t)irq10, 0x8, 0x8E);
	idt_set(43, (uint32_t)irq11, 0x8, 0x8E);
	idt_set(44, (uint32_t)irq12, 0x8, 0x8E);
	idt_set(45, (uint32_t)irq13, 0x8, 0x8E);
	idt_set(46, (uint32_t)irq14, 0x8, 0x8E);
	idt_set(47, (uint32_t)irq15, 0x8, 0x8E);

	idt_write((uint32_t)&idt_ptr);

	return 256;
}

void idt_set(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
	idt_table[num].base_lo = base & 0xffff;
	idt_table[num].base_hi = (base >> 16) & 0xffff;

	idt_table[num].sel = sel;
	idt_table[num].reserved = 0;
	idt_table[num].flags = flags /* | 0x60 */;
}
