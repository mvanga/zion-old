.section .text

.globl gdt_write
.globl idt_write
.globl phys_frame_clone

gdt_write:
	mov 4(%esp), %eax
	lgdt (%eax)
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	jmp $0x08, $flush
flush:
	ret


idt_write:
	mov 4(%esp), %eax
	lidt (%eax)
	ret

phys_frame_clone:
	push %ebx
	pushf
	cli

	mov 12(%esp), %ebx
	mov 16(%esp), %ecx

	mov %cr0, %edx
	and $0x7fffffff, %edx
	mov %edx, %cr0

	mov $1024, %edx

copy_loop:
	mov (%ebx), %eax
	mov %eax, (%ecx)
	add $0x4, %ebx
	add $0x4, %ecx
	dec %edx
	jnz copy_loop

	mov %cr0, %edx
	or $0x80000000, %edx
	mov %edx, %cr0

	popf
	pop %ebx
	ret

.macro ISR_NOERRCODE n
	.globl isr\n
	isr\n:
		cli
		push $0xabad1dea
		push $\n
		jmp isr_stub
.endm

.macro ISR_ERRCODE n
	.globl isr\n
	isr\n:
		cli
		push $\n
		jmp isr_stub
.endm

.extern isr_handler
isr_stub:
	pusha
	push %ds
	push %es
	push %fs
	push %gs
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %esp, %eax
	push %eax
	call isr_handler
	pop %eax
	pop %gs
	pop %fs
	pop %es
	pop %ds
	popa
	add $0x8, %esp
	iret

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

.macro IRQ num, irqnum
	.globl irq\num
	irq\num:
		cli
		push $0xabad1dea
		push $\irqnum
		jmp irq_stub
.endm

.extern irq_handler
irq_stub:
	pusha
	push %ds
	push %es
	push %fs
	push %gs
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %esp, %eax
	push %eax
	call irq_handler
	pop %eax
	pop %gs
	pop %fs
	pop %es
	pop %ds
	popa
	add $0x8, %esp
	iret

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47
