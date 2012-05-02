#ifndef ARCH_X86_ASM_IRQ_H
#define ARCH_X86_ASM_IRQ_H

#include <asm/regs.h>

typedef void (*isr_t)(struct regs *);
extern isr_t irq_handlers[256];

void request_exception(int n, isr_t handler);
void request_irq(int n, isr_t handler);
void irq_disable(unsigned char irq);
void irq_enable(unsigned char irq);

#endif
