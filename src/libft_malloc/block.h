#ifndef LIBFT_MALLOC_BLOCK_H
#define LIBFT_MALLOC_BLOCK_H

#include <stdbool.h>
#include <stddef.h>

typedef struct zones_s zones_t;

typedef struct block_s block_t;
struct block_s
{
	block_t *prev;
	bool is_free;
	size_t size;
};

#define BLOCK_START(block) ((void *)(block) + sizeof(block_t))
#define BLOCK_NEXT(block) ((void *)(block) + sizeof(block_t) + (block)->size)

void *allocateFreeBlock(zones_t *zone, block_t *free_block, size_t size);
void freeBlock(zones_t *zone, block_t *block);
block_t *blocksSearchPtr(const zones_t *zone, const void *ptr);

#endif
