.section .text

.equ MBOOT_PAGE_ALIGN,    1<<0
.equ MBOOT_MEM_INFO,      1<<1
.equ MBOOT_HEADER_MAGIC,  0x1BADB002
.equ MBOOT_HEADER_FLAGS,  MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
.equ MBOOT_CHECKSUM,      -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


.globl mboot
.extern __text_start
.extern __bss_start
.extern __bss_end

mboot:
	.long MBOOT_HEADER_MAGIC
	.long MBOOT_HEADER_FLAGS
	.long MBOOT_CHECKSUM
	
	/*
	.long mboot
	.long __text_start
	.long __bss_start
	.long __bss_end
	.long start
	*/

.globl start
.extern main

start:
	/*
	 * here's the trick: we load a GDT with a base address
	 * of 0x40000000 for the code (0x08) and data (0x10) segments
	 */
	cli
	mov trickgdt, %eax
	lgdt (%eax)
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	
	/* jump to the higher half kernel */
	jmp $0x08, $higherhalf

higherhalf:
	/*
	 * from now the CPU will translate automatically every address
	 * by adding the base 0x40000000
	 */
	mov $_sys_stack, %esp
	call arch_main
loop:
	jmp loop


.section .bss
	.lcomm _sys_stack, 8192

.section .setup
trickgdt:
	.word gdt_end - gdt - 1
	.long gdt

gdt:
	.long 0, 0						/* null gate */
	.byte 0xff, 0xff, 0, 0, 0, 0x9a, 0xcf, 0x40	/* code selector 0x08: base 0x40000000, limit 0xFFFFFFFF, type 0x9A, granularity 0xCF */
	.byte 0xff, 0xff, 0, 0, 0, 0x92, 0xcf, 0x40	/* data selector 0x10: base 0x40000000, limit 0xFFFFFFFF, type 0x92, granularity 0xCF */
gdt_end:
