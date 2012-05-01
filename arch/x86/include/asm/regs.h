#ifndef ARCH_X86_ASM_REGS_H
#define ARCH_X86_ASM_REGS_H

#include <zion/types.h>

struct regs {
	uint32_t ds;
        uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t int_no;
	uint32_t err_code;
	uint32_t ss;
	uint32_t useresp;
	uint32_t eflags;
	uint32_t cs;
	uint32_t eip;
};

#endif
