#include <assert.h>
#include <limits.h>

#include "libft_malloc/block.h"
#include "libft_malloc/memory.h"
#include "libft_malloc/zones.h"
#include "libft_malloc/utils/print.h"
#include "libft_malloc/utils/string.h"

void *allocateFreeBlock(zones_t *zone, block_t *free_block, size_t size)
{
	assert(zone != NULL);
	assert(free_block != NULL);
	assert(free_block->is_free == true);
	assert(free_block->size >= size);
	assert(size != 0);

	free_block->is_free = false;
	size_t unused = free_block->size - size;
	if (unused > sizeof(block_t)) {
		free_block->size = size;
		block_t *next = BLOCK_NEXT(free_block);
		next->prev = free_block;
		next->is_free = true;
		next->size = unused - sizeof(block_t);
	}
	if (free_block == zone->leftmost_free_block) {
		zone->leftmost_free_block = BLOCK_NEXT(free_block);
	}
	zone->block_used_count++;
	if (memory.debug_variables.perturb_byte != 0) {
		memorySet(BLOCK_START(free_block), memory.debug_variables.perturb_byte,
				free_block->size);
	}
	return BLOCK_START(free_block);
}

void freeBlock(zones_t *zone, block_t *block)
{
	assert(zone != NULL);
	assert(isPtrInZone(zone, block) == true);

	if (memory.debug_variables.perturb_byte != 0) {
		memorySet(BLOCK_START(block), UCHAR_MAX - memory.debug_variables.perturb_byte,
				block->size);
	}
	block_t *next = BLOCK_NEXT(block);
	bool is_next_in_zone = isPtrInZone(zone, next);
	if (is_next_in_zone == true && next->is_free == true) {
		block->size += sizeof(block_t) + next->size;
		next = BLOCK_NEXT(next);
		is_next_in_zone = isPtrInZone(zone, next);
	}
	if (block->prev != NULL && block->prev->is_free == true) {
		block->prev->size += sizeof(block_t) + block->size;
		block = block->prev;
	} else {
		block->is_free = true;
	}

	if (is_next_in_zone == true) {
		next->prev = block;
	}
	if (block < zone->leftmost_free_block) {
		zone->leftmost_free_block = block;
	}
	zone->block_used_count--;
}

block_t *blocksSearchPtr(const zones_t *zone, const void *ptr)
{
	assert(ptr != NULL);
	assert(isPtrInZone(zone, ptr) == true);

	block_t *block = ZONE_START(zone);
	void *zone_end = ZONE_END(zone);
	while ((void *)block < zone_end) {
		if (BLOCK_START(block) == ptr) {
			return block;
		}
		block = BLOCK_NEXT(block);
	}
	return NULL;
}
