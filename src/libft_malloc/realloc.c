#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "libft_malloc/larges.h"
#include "libft_malloc/malloc.h"
#include "libft_malloc/free.h"
#include "libft_malloc/memory.h"
#include "libft_malloc/block.h"
#include "libft_malloc/utils/math.h"
#include "libft_malloc/utils/string.h"
#include "libft_malloc/zones.h"

static void *reallocLarge(larges_t *large, void *ptr, size_t size)
{
	assert(large != NULL);
	assert(ptr != NULL);
	assert(size != 0);

	if (size <= large->size) {
		return ptr;
	}

	void *new = mallocImplementation(size, DontAddHistoryEntry);
	if (new == NULL) {
		return NULL;
	}
	memoryCopy(new, ptr, min(size, large->size));
	largesDelete(&memory.larges, large);
	return new;
}

static void *reallocZoneAllocation(zones_t **zones, zones_t *zone, void *ptr, size_t size)
{
	assert(zone != NULL);
	assert(ptr != NULL);
	assert(size != 0);

	block_t *block = blocksSearchPtr(zone, ptr);
	if (block == NULL) {
		return NULL;
	}

	if (size == block->size) {
		return ptr;
	}
	if (size > block->size) {
		void *new = mallocImplementation(size, DontAddHistoryEntry);
		if (new == NULL) {
			return NULL;
		}
		memoryCopy(new, ptr, min(size, block->size));
		freeBlock(zone, block);
		if (zone->block_used_count == 0) {
			zonesDelete(zones, zone);
		}
		return new;
	}
	// size is inferior to block->size
	size_t unused = block->size - size;
	block_t *next = BLOCK_NEXT(block);
	bool is_next_in_zone = isPtrInZone(zone, next);
	if (unused <= sizeof(block_t)
		&& (is_next_in_zone == false || next->is_free == false)) {
		// Too little space left to create a new block
		if (memory.debug_variables.perturb_byte != 0) {
			memorySet(BLOCK_START(block) + size, UCHAR_MAX - memory.debug_variables.perturb_byte,
					unused);
		}
		return ptr;
	}
	// There is enough space in block or in next to create a new block
	block->size = size;
	block_t *new = BLOCK_NEXT(block);
	new->prev = block;
	new->is_free = true;
	if (is_next_in_zone == false || next->is_free == false) {
		// There is no space in next to create a new block but there is in block
		new->size = unused - sizeof(block_t);
		if (is_next_in_zone == true) {
			next->prev = new;
		}
		return ptr;
	}
	// There is enough space in next to create a new block
	size_t next_size = next->size;
	new->size = unused - sizeof(block_t) + sizeof(block_t) + next_size;
	block_t *next_next = BLOCK_NEXT(next);
	if (isPtrInZone(zone, next_next)) {
		next_next->prev = new;
	}
	return ptr;
}

static void *reallocReturn(void *ptr, size_t size)
{
	if (ptr == NULL) {
		errno = ENOMEM;
		return NULL;
	}
	allocationHistoriesAddEntry(&memory.histories,
			(allocation_histories_entry_t){ .size = size, .is_a_reallocation = true });
	return ptr;
}

void *reallocImplementation(void *ptr, size_t size)
{
	if (ptr == NULL) {
		return mallocImplementation(size, AddHistoryEntry);
	}
	if (size == 0) {
		freeImplementation(ptr);
		return mallocImplementation(0, AddHistoryEntry);
	}

	larges_t *large = largesSearchPtr(memory.larges, ptr);
	if (large != NULL) {
		return reallocReturn(reallocLarge(large, ptr, size), size);
	}
	zones_t *zone;
	zone = zonesSearchPtr(memory.tinies, ptr);
	if (zone != NULL) {
		return reallocReturn(reallocZoneAllocation(&memory.tinies, zone, ptr, size), size);
	}
	zone = zonesSearchPtr(memory.smalls, ptr);
	if (zone != NULL) {
		return reallocReturn(reallocZoneAllocation(&memory.smalls, zone, ptr, size), size);
	}
	return NULL;
}

void *realloc(void *ptr, size_t size)
{
	if (pthread_mutex_lock(&memory_mutex) != 0) {
		return NULL;
	}
	if (memory.debug_variables.is_initialized == false) {
		setDebugVariables(&memory.debug_variables);
	}
	ptr = reallocImplementation(ptr, size);
	if (pthread_mutex_unlock(&memory_mutex) != 0) {
		assert(!"pthread_mutex_unlock EPERM error");
	}
	return ptr;
}
