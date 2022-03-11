#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "libft_malloc/debug_variables.h"
#include "libft_malloc/memory.h"
#include "libft_malloc/allocation_histories.h"
#include "libft_malloc/larges.h"
#include "libft_malloc/utils/print.h"
#include "libft_malloc/zones.h"
#include "libft_malloc/block.h"
#include "libft_malloc/free.h"

memory_t memory = {
	.tinies = NULL,
	.smalls = NULL,
	.larges = NULL,
	.histories = NULL,
#ifdef ENABLE_DEBUG_VARIABLES
	.debug_variables = {
		.is_initialized = false,
		.perturb_byte = 0,
	},
#endif
};

pthread_mutex_t memory_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *getZoneAllocation(zones_t **zones, size_t allocation_max_size, size_t size)
{
	assert(zones != NULL);
	assert(allocation_max_size != 0);
	assert(size != 0);

	while (*zones != NULL) {
		block_t *block = (*zones)->leftmost_free_block;
		void *zone_end = ZONE_END(*zones);
		while ((void *)block < zone_end) {
			if (block->is_free == true && block->size >= size) {
				void *ptr = allocateFreeBlock(*zones, block, size);
				return ptr;
			}
			block = BLOCK_NEXT(block);
		}
		zones = &(*zones)->next;
	}

	*zones = zonesCreate(allocation_max_size);
	if (*zones == NULL) {
		return NULL;
	}
	void *ptr = allocateFreeBlock(*zones, (*zones)->leftmost_free_block, size);
	return ptr;
}

static void *getLargeAllocation(larges_t **larges, size_t size)
{
	assert(larges != NULL);
	assert(size != 0);

	while (*larges != NULL) {
		larges = &(*larges)->next;
	}
	*larges = largesCreate(size);
	if (*larges == NULL) {
		return NULL;
	}
	return LARGE_START(*larges);
}

void *mallocImplementation(size_t size, allocation_history_action_t allocation_history_action)
{
	assert(isAllocationHistoryActionValid(allocation_history_action) == true);

	if (size == 0) {
		return NULL;
	}

	void *ptr = NULL;
	if (size <= TINY_MAX_SIZE) {
		ptr = getZoneAllocation(&memory.tinies, TINY_MAX_SIZE, size);
	} else if (size <= SMALL_MAX_SIZE) {
		ptr = getZoneAllocation(&memory.smalls, SMALL_MAX_SIZE, size);
	} else {
		ptr = getLargeAllocation(&memory.larges, size);
	}
	if (ptr == NULL) {
		errno = ENOMEM;
		return NULL;
	}
	if (allocation_history_action == AddHistoryEntry) {
		allocationHistoriesAddEntry(&memory.histories,
				(allocation_histories_entry_t){ .size = size, .is_a_reallocation = false });
	}
	return ptr;
}

void *malloc(size_t size)
{
	if (pthread_mutex_lock(&memory_mutex) != 0) {
		return NULL;
	}
#ifdef ENABLE_DEBUG_VARIABLES
	if (memory.debug_variables.is_initialized == false) {
		setDebugVariables(&memory.debug_variables);
	}
	memory.allocations_count++;
	if (memory.debug_variables.fail_at != 0) {
		if (memory.debug_variables.fail_at == memory.allocations_count) {
			if (pthread_mutex_unlock(&memory_mutex) != 0) {
				assert(!"pthread_mutex_unlock EPERM error");
			}
			return NULL;
		}
	}
#endif
	void *ptr = mallocImplementation(size, AddHistoryEntry);
	if (pthread_mutex_unlock(&memory_mutex) != 0) {
		assert(!"pthread_mutex_unlock EPERM error");
	}
	return ptr;
}
