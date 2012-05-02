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
	
	.long mboot
	.long __text_start
	.long __bss_start
	.long __bss_end
	.long start

.globl start
.extern main

start:
	cli
	mov $_sys_stack, %esp
	call arch_main
loop:
	jmp loop


.section .bss
	.lcomm _sys_stack, 8192
