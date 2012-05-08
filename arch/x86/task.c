#include <asm/task.h>
#include <zion/alloc.h>
#include <zion/stdio.h>
#include <zion/stdlib.h>

volatile struct task *current;

struct task *task_queue = NULL;

static uint32_t next_pid = 0;

extern uint32_t read_eip(void);

void task_init(void)
{
	struct task *init;

	asm volatile("cli");

	init = kmalloc(sizeof(struct task), ALLOC_KERN);
	if (!init) {
		printk("init task alloc failed\n");
		for (;;);
	}
	init->pid = next_pid++;
	init->esp = 0;
	init->ebp = 0;
	init->eip = 0;
	init->pdir = curr_page_dir;
	init->next = NULL;

	task_queue = init;
	current = init;

	asm volatile("sti");
}

int fork(void)
{
	uint32_t eip;
	struct task *new;
	struct task *parent;
	struct page_dir *dir;

	asm volatile ("cli");

	parent = (struct task *)current;
	dir = page_dir_clone(curr_page_dir);

	new = kmalloc(sizeof(struct task), ALLOC_KERN);
	if (!new)
		return -1;

	new->pid = next_pid++;
	new->esp = 0;
	new->ebp = 0;
	new->eip = 0;
	new->pdir = dir;

	new->next = task_queue;
	task_queue = new;

	eip = read_eip();

	if (current == parent) {
		uint32_t esp;
		uint32_t ebp;
		asm volatile("mov %%esp, %0" : "=r"(esp));
		asm volatile("mov %%ebp, %0" : "=r"(ebp));
		new->esp = esp;
		new->ebp = ebp;
		new->eip = eip;
		asm volatile ("sti");

		return new->pid;
	}

	return 0;
}

int getpid(void)
{
	return current->pid;
}

struct task *task_get_next(void)
{
	struct task *t;

	t = task_queue;
	t = t->next;
	if (!t)
		t = task_queue;
	return t;
}

void task_queue_dump(void)
{
	struct task *t;

	t = task_queue;
	while (t) {
		printk("task: pid=%d,eip=%08x,ebp=%08x,esp=%08x\n", t->pid, t->eip, t->ebp, t->esp);
		t = t->next;
	}
}

void schedule(void)
{
	uint32_t esp;
	uint32_t ebp;
	uint32_t eip;

	asm volatile("mov %%esp, %0" : "=r"(esp));
	asm volatile("mov %%ebp, %0" : "=r"(ebp));

	if (!current)
		return;

	eip = read_eip();

	if (eip == 0xdeaddead) {
	//	printk("WHEE\n");
	//	task_queue_dump();
		return;
	}

	current->eip = eip;
	current->esp = esp;
	current->ebp = ebp;

	current = task_get_next();

	eip = current->eip;
	ebp = current->ebp;
	esp = current->esp;

	printk("eip=%p,ebp=%p,esp=%p\n", eip, ebp, esp);

	curr_page_dir = current->pdir;

	asm volatile("         \
		cli;                 \
		mov %0, %%ecx;       \
		mov %1, %%esp;       \
		mov %2, %%ebp;       \
		mov %3, %%cr3;       \
		mov $0xdeaddead, %%eax; \
		sti;                 \
		jmp *%%ecx           "
		: : "r"(eip), "r"(esp), "r"(ebp), "r"(curr_page_dir->tables_phys_addr));
}
