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
#include "libft_malloc/utils/print.h"
#include "libft_malloc/zones.h"

static void *reallocLarge(larges_t *large, void *ptr, size_t size)
{
	assert(large != NULL);
	assert(ptr != NULL);
	assert(size != 0);

	if (size <= large->size) {
		return ptr;
	}

#ifdef ENABLE_DEBUG_VARIABLES
	memory.used_bytes_count -= large->size;
#endif
#ifdef ENABLE_DEBUG_VARIABLES
	void *new = mallocImplementation(size, DontAddHistoryEntry);
#else
	void *new = mallocImplementation(size);
#endif
#ifdef ENABLE_DEBUG_VARIABLES
	memory.used_bytes_count += large->size;
#endif
	if (new == NULL) {
		return NULL;
	}
	memoryCopy(new, ptr, min(size, large->size));
	largesDelete(&memory.larges, large);
	return new;
}

static void *smallerReallocZoneAllocation(zones_t *zone, void *ptr, size_t size,
		block_t *block)
{
	size_t unused = block->size - size;
	block_t *next = BLOCK_NEXT(block);
	bool is_next_in_zone = isPtrInZone(zone, next);
	if (unused <= sizeof(block_t)
		&& (is_next_in_zone == false || next->is_free == false)) {
		// Too little space left to create a new block
#ifdef ENABLE_DEBUG_VARIABLES
		if (memory.debug_variables.perturb_byte != 0) {
			memorySet(BLOCK_START(block) + size, UCHAR_MAX - memory.debug_variables.perturb_byte,
					unused);
		}
#endif
		return ptr;
	}
	// There is enough space in block or in next to create a new block
#ifdef ENABLE_DEBUG_VARIABLES
	memory.used_bytes_count -= block->size - size;
#endif
	block->size = size;
	block_t *new = BLOCK_NEXT(block);
	new->prev = block;
	new->is_free = true;
	if (new < zone->leftmost_free_block_candidate) {
		zone->leftmost_free_block_candidate = new;
	}
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
	block_t *next_next = BLOCK_NEXT(next);
	new->size = unused - sizeof(block_t) + sizeof(block_t) + next_size;
	if (isPtrInZone(zone, next_next)) {
		next_next->prev = new;
	}
	return ptr;
}

static void *largerReallocZoneAllocation(zones_t **zones, zones_t *zone, void *ptr, size_t size,
		block_t *block)
{
	block_t *next = BLOCK_NEXT(block);
	if (isPtrInZone(zone, next) == true && next->is_free == true
		&& size <= block->size + sizeof(block_t) + next->size) {
		block->size += sizeof(block_t) + next->size;
		block_t *next_next = BLOCK_NEXT(next);
		if (next == zone->leftmost_free_block_candidate) {
			zone->leftmost_free_block_candidate = getNextFreeBlock(zone, next);
		}
		if (isPtrInZone(zone, next_next) == true) {
			next_next->prev = block;
		}
		if (size == block->size) {
			return ptr;
		}
		return smallerReallocZoneAllocation(zone, ptr, size, block);
	}

#ifdef ENABLE_DEBUG_VARIABLES
	memory.used_bytes_count -= block->size;
#endif
#ifdef ENABLE_DEBUG_VARIABLES
	void *new = mallocImplementation(size, DontAddHistoryEntry);
#else
	void *new = mallocImplementation(size);
#endif
#ifdef ENABLE_DEBUG_VARIABLES
	memory.used_bytes_count += block->size;
#endif
	if (new == NULL) {
		return NULL;
	}
	memoryCopy(new, ptr, min(size, block->size));
	freeBlock(zone, block);
	if (zone->used_blocks_count == 0) {
		zonesDelete(zones, zone);
	}
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
	} else if (size < block->size) {
		return smallerReallocZoneAllocation(zone, ptr, size, block);
	} else {
		return largerReallocZoneAllocation(zones, zone, ptr, size, block);
	}
}

#ifdef ENABLE_DEBUG_VARIABLES
static void *reallocReturn(void *ptr, size_t size)
#else
static void *reallocReturn(void *ptr)
#endif
{
	if (ptr == NULL) {
		errno = ENOMEM;
		return NULL;
	}
#ifdef ENABLE_DEBUG_VARIABLES
	if (memory.debug_variables.enable_history == true) {
		allocationHistoriesAddEntry(&memory.histories,
				(allocation_histories_entry_t){ .size = size, .is_a_reallocation = true });
	}
#endif
	return ptr;
}

void *reallocImplementation(void *ptr, size_t size)
{
	if (ptr == NULL) {
#ifdef ENABLE_DEBUG_VARIABLES
		return mallocImplementation(size, AddHistoryEntry);
#else
		return mallocImplementation(size);
#endif
	}
	if (size == 0) {
		freeImplementation(ptr);
#ifdef ENABLE_DEBUG_VARIABLES
		return mallocImplementation(0, AddHistoryEntry);
#else
		return mallocImplementation(0);
#endif
	}
#ifdef ENABLE_DEBUG_VARIABLES
	if (memory.debug_variables.fail_at != 0) {
		if (memory.debug_variables.fail_at == memory.allocations_count) {
			errno = ENOMEM;
			return NULL;
		}
	}
#endif
	larges_t *large = largesSearchPtr(memory.larges, ptr);
	if (large != NULL) {
#ifdef ENABLE_DEBUG_VARIABLES
		return reallocReturn(reallocLarge(large, ptr, size), size);
#else
		return reallocReturn(reallocLarge(large, ptr, size));
#endif
	}
	zones_t *zone;
	zone = zonesSearchPtr(memory.tinies, ptr);
	if (zone != NULL) {
#ifdef ENABLE_DEBUG_VARIABLES
		return reallocReturn(reallocZoneAllocation(&memory.tinies, zone, ptr, size), size);
#else
		return reallocReturn(reallocZoneAllocation(&memory.tinies, zone, ptr, size));
#endif
	}
	zone = zonesSearchPtr(memory.smalls, ptr);
	if (zone != NULL) {
#ifdef ENABLE_DEBUG_VARIABLES
		return reallocReturn(reallocZoneAllocation(&memory.smalls, zone, ptr, size), size);
#else
		return reallocReturn(reallocZoneAllocation(&memory.smalls, zone, ptr, size));
#endif
	}
	return NULL;
}

void *realloc(void *ptr, size_t size)
{
	if (pthread_mutex_lock(&memory_mutex) != 0) {
		return NULL;
	}
#ifdef ENABLE_DEBUG_VARIABLES
	if (memory.debug_variables.is_initialized == false) {
		setDebugVariables(&memory.debug_variables);
	}
	memory.allocations_count++;
#endif
	ptr = reallocImplementation(ptr, size);
	if (pthread_mutex_unlock(&memory_mutex) != 0) {
		assert(!"pthread_mutex_unlock EPERM error");
	}
	return ptr;
}
