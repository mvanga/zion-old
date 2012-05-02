#include <zion/bitset.h>
#include <zion/stdlib.h>
#include <zion/types.h>
#include <zion/string.h>
#include <zion/stdio.h>

#define BIT_INDEX(a) (a/(8*sizeof(uint32_t)))
#define BIT_OFFSET(a) (a%(8*sizeof(uint32_t)))

#if 0
struct bitset *bitset_create(int nbits)
{
	int len;
	struct bitset *b;

	len = (nbits/(sizeof(uint32_t)*8)) +
		((nbits%(sizeof(uint32_t)*8)) ? 1 : 0);

	b = kmalloc(sizeof(struct bitset));
	if (!b)
		return NULL;
	b->bits = kmalloc(len * sizeof(uint32_t));
	if (!b->bits) {
		kfree(b);
		return NULL;
	}
	memset(b->bits, 0, len*sizeof(uint32_t));
	b->len = len;

	return b;
}
#endif

void bit_set(struct bitset *b, int bit)
{
	uint32_t idx;
	uint32_t off;

	if (!b)
		return;
	
	idx = BIT_INDEX(bit);
	off = BIT_OFFSET(bit);
	if (idx > b->len)
		return;
	b->bits[idx] |= (1 << off);
}

void bit_clear(struct bitset *b, int bit)
{
	uint32_t idx;
	uint32_t off;

	if (!b)
		return;
	idx = BIT_INDEX(bit);
	off = BIT_OFFSET(bit);
	if (idx > b->len)
		return;
	b->bits[idx] &= ~(1 << off);
}

int bit_test(struct bitset *b, int bit)
{
	uint32_t idx;
	uint32_t off;

	idx = BIT_INDEX(bit);
	off = BIT_OFFSET(bit);
	return ((b->bits[idx] & (1 << off)) ? 1 : 0);
}

int bit_find_first_free(struct bitset *b)
{
	uint32_t i, j;

	if (!b)
		return -1;

	for (i = 0; i < b->len; i++) {
		if (b->bits[i] == 0xffffffff)
			continue;
		for (j = 0; j < 32; j++)
			if (!(b->bits[i] & (1 << j)))
				return i*32 + j;
	}
	return -1;
}

void bitset_print(struct bitset *b)
{
	int i;

	for (i = b->len*32 - 1; i >= 0; i--) {
		printk("%d", bit_test(b, i));
		if (i % 8 == 0)
			printk(" ");
	}
	printk("\n");
}
