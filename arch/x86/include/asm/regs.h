#ifndef ARCH_X86_ASM_REGS_H
#define ARCH_X86_ASM_REGS_H

#include <zion/types.h>

#if 0
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

struct regs
{
	unsigned int gs, fs, es, ds;      /* pushed the segs last */
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
	unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
	unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

#endif
