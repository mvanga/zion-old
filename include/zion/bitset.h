#ifndef ZION_BITSET_H
#define ZION_BITSET_H

#include <zion/types.h>

struct bitset {
	uint32_t len;
	uint32_t *bits;
};

/* Create a bitset */
struct bitset *bitset_create(int nbits);
/* Bitset operations */
void bit_set(struct bitset *b, int bit);
void bit_clear(struct bitset *b, int bit);
int bit_test(struct bitset *b, int bit);
int bit_find_first_free(struct bitset *b);

void bitset_print(struct bitset *b);

#endif
