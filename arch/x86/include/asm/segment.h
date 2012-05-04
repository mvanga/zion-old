#ifndef ARCH_X86_ASM_GDT_H
#define ARCH_X86_ASM_GDT_H

#include <zion/types.h>

struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_middle;
	uint8_t  access;
	uint8_t  granularity;
	uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

void gdt_init(void);
void gdt_set(int32_t num, uint32_t base, uint32_t limit,
		uint8_t access, uint8_t gran);

struct idt_entry {
	uint16_t base_lo;
	uint16_t sel;
	uint8_t  reserved;
	uint8_t  flags;
	uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

int idt_init(void);
void idt_set(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

#endif
