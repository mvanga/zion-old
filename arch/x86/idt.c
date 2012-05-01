#include <asm/segment.h>

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

void debug(uint32_t a, uint32_t b) { printk("%u %u\n", a, b); }
void debug2(uint32_t a, uint32_t b) { printk("%u %u\n", a, b); }

struct idt_entry idt_table[256];
struct idt_ptr idt_ptr;

void idt_init(void)
{
	idt_ptr.limit = sizeof(struct idt_entry) * 256 -1;
	idt_ptr.base  = (uint32_t)&idt_table;

	memset(&idt_table, 0, sizeof(struct idt_entry) * 256);

	idt_set(0, (uint32_t)isr0 ,0x08, 0x8E);
	idt_set(1, (uint32_t)isr1 ,0x08, 0x8E);
	idt_set(2, (uint32_t)isr2 ,0x08, 0x8E);
	idt_set(3, (uint32_t)isr3 ,0x08, 0x8E);
	idt_set(4, (uint32_t)isr4 ,0x08, 0x8E);
	idt_set(5, (uint32_t)isr5 ,0x08, 0x8E);
	idt_set(6, (uint32_t)isr6 ,0x08, 0x8E);
	idt_set(7, (uint32_t)isr7 ,0x08, 0x8E);
	idt_set(8, (uint32_t)isr8 ,0x08, 0x8E);
	idt_set(9, (uint32_t)isr9 ,0x08, 0x8E);
	idt_set(10, (uint32_t)isr10 ,0x08, 0x8E);
	idt_set(11, (uint32_t)isr11 ,0x08, 0x8E);
	idt_set(12, (uint32_t)isr12 ,0x08, 0x8E);
	idt_set(13, (uint32_t)isr13 ,0x08, 0x8E);
	idt_set(14, (uint32_t)isr14 ,0x08, 0x8E);
	idt_set(15, (uint32_t)isr15 ,0x08, 0x8E);
	idt_set(16, (uint32_t)isr16 ,0x08, 0x8E);
	idt_set(17, (uint32_t)isr17 ,0x08, 0x8E);
	idt_set(18, (uint32_t)isr18 ,0x08, 0x8E);
	idt_set(19, (uint32_t)isr19 ,0x08, 0x8E);
	idt_set(20, (uint32_t)isr20 ,0x08, 0x8E);
	idt_set(21, (uint32_t)isr21 ,0x08, 0x8E);
	idt_set(22, (uint32_t)isr22 ,0x08, 0x8E);
	idt_set(23, (uint32_t)isr23 ,0x08, 0x8E);
	idt_set(24, (uint32_t)isr24 ,0x08, 0x8E);
	idt_set(25, (uint32_t)isr25 ,0x08, 0x8E);
	idt_set(26, (uint32_t)isr26 ,0x08, 0x8E);
	idt_set(27, (uint32_t)isr27 ,0x08, 0x8E);
	idt_set(28, (uint32_t)isr28 ,0x08, 0x8E);
	idt_set(29, (uint32_t)isr29 ,0x08, 0x8E);
	idt_set(30, (uint32_t)isr30 ,0x08, 0x8E);
	idt_set(31, (uint32_t)isr31 ,0x08, 0x8E);

	idt_write((uint32_t)&idt_ptr);
}

void idt_set(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
	idt_table[num].base_lo = base & 0xffff;
	idt_table[num].base_hi = (base >> 16) & 0xffff;

	idt_table[num].sel = sel;
	idt_table[num].reserved = 0;
	idt_table[num].flags = flags /* | 0x60 */;
}
