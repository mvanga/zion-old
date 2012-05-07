#include <zion/types.h>
#include <zion/stdlib.h>

#define BLOCKSIZE		(sizeof(struct alloc_area) - 1)

#define ALLOC_FLAG_FREE		(1 << 0)
#define ALLOC_MAGIC		(0xdecaffe0)
#define ALLOC_MAGIC_MASK	(0xfffffff0)

struct alloc_area {
	uint32_t flags;
	uint32_t size;
	struct alloc_area *next;
	struct alloc_area *prev;
	/* start of data */
	char data[1];
} __attribute__((packed));

struct heap {
	struct alloc_area *alloc;
	uint32_t start;
	uint32_t brk;
	uint32_t max;
};

struct alloc_area *alloc_init(void *start, uint32_t size)
{
	struct alloc_area *begin;

	begin = (struct alloc_area *)start;
	begin->flags = ALLOC_MAGIC | ALLOC_FLAG_FREE;
	begin->size = size - BLOCKSIZE;
	begin->next = NULL;
	begin->prev = NULL;

	return begin;
}

/* either breaks region or not. return pointer */
void *alloc_area_split(struct alloc_area *area, int size)
{
	struct alloc_area *new;
	void *ptr;

	if (area->size < (size + BLOCKSIZE + 4))
		return (void *)area->data;

	ptr = (void *)area->data;
	new = (struct alloc_area *)((char *)ptr + size);
	new->flags = ALLOC_MAGIC | ALLOC_FLAG_FREE;
	new->size = area->size - size - BLOCKSIZE;
	new->next = area->next;
	new->prev = area;
	area->next = new;
	if (new->next)
		new->next->prev = new;

	return ptr;
}

void alloc_area_merge(struct alloc_area *area, struct alloc_area *main)
{
	struct alloc_area *t;

	t = area;
	while (1) {
		if (t->next && (t->next->flags & ALLOC_FLAG_FREE)) {
			t->size = t->size + BLOCKSIZE + t->next->size;
			t->next = t->next->next;
			if (t->next)
				t->next->prev = t;
		}
		t->flags &= ~ALLOC_FLAG_FREE;
		if (t == main)
			break;
		t = t->prev;
	}
}

void *alloc(struct alloc_area *area, int size)
{
	struct alloc_area *t;

	t = area;
	while (t) {
		if (t->size >= (uint32_t)size)
			return alloc_area_split(t, size);
		t = t->next;
	}
	return NULL;
}

void free(struct alloc_area *area, void *ptr)
{
	struct alloc_area *t;

	t = (struct alloc_area *)((char *)ptr - BLOCKSIZE);
	if ((t->flags & ALLOC_MAGIC_MASK) != ALLOC_MAGIC)
		return;

	alloc_area_merge(t, area);
}

struct heap kern_heap;

void kernel_heap_init(uint32_t start, uint32_t brk, uint32_t max)
{
	kern_heap.start = start;
	kern_heap.brk = brk;
	kern_heap.max = max;
	kern_heap.alloc = (struct alloc_area *)((char *)start);
	kern_heap.alloc = alloc_init((void *)start, brk - start);
}

void *kmalloc(uint32_t size)
{
	return alloc(kern_heap.alloc, size);
}

void kfree(void *p)
{
	free(kern_heap.alloc, p);
}
