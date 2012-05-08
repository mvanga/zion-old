#ifndef ZION_ASM_TASK_H
#define ZION_ASM_TASK_H

#include <zion/list.h>
#include <zion/types.h>

#include <asm/paging.h>

struct task {
	int pid;
	uint32_t esp;
	uint32_t ebp;
	uint32_t eip;
	struct page_dir *pdir;
	struct list_head list;
	struct task *next;
};

void task_init(void);
int fork(void);
int getpid(void);
void schedule(void);

#endif
