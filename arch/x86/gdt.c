#include <asm/segment.h>

extern void gdt_write(uint32_t);

struct gdt_ptr gdt_ptr;
struct gdt_entry gdt_table[5];

void gdt_set(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
	gdt_table[num].base_low = (base & 0xFFFF);
	gdt_table[num].base_middle = (base >> 16) & 0xFF;
	gdt_table[num].base_high = (base >> 24) & 0xFF;

	gdt_table[num].limit_low = (limit & 0xFFFF);
	gdt_table[num].granularity = (limit >> 16) & 0x0F;

	gdt_table[num].granularity |= gran & 0xF0;
	gdt_table[num].access = access;
}

void gdt_init(void)
{
	gdt_ptr.limit = (sizeof(struct gdt_entry) * 5) - 1;
	gdt_ptr.base  = (uint32_t)&gdt_table;

	gdt_set(0, 0, 0, 0, 0);                // Null segment
	gdt_set(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	gdt_set(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	gdt_set(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	gdt_set(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

	gdt_write((uint32_t)&gdt_ptr);
}
