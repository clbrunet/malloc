#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "libft_malloc/free.h"
#include "libft_malloc/block.h"
#include "libft_malloc/larges.h"
#include "libft_malloc/memory.h"
#include "libft_malloc/utils/print.h"
#include "libft_malloc/zones.h"

static void freeZoneAllocation(zones_t **zones, zones_t *zone, void *ptr)
{
	assert(zone != NULL);
	assert(ptr != NULL);

	block_t *block = blocksSearchPtr(zone, ptr);
	if (block == NULL) {
		return;
	}
	if (block->is_free == false) {
		freeBlock(zone, block);
	}
	if (zone->used_blocks_count == 0) {
		zonesDelete(zones, zone);
	}
}

void freeImplementation(void *ptr)
{
	if (ptr == NULL) {
		return;
	}

	larges_t *large = largesSearchPtr(memory.larges, ptr);
	if (large != NULL) {
		largesDelete(&memory.larges, large);
		return;
	}
	zones_t *zone;
	zone = zonesSearchPtr(memory.tinies, ptr);
	if (zone != NULL) {
		freeZoneAllocation(&memory.tinies, zone, ptr);
		return;
	}
	zone = zonesSearchPtr(memory.smalls, ptr);
	if (zone != NULL) {
		freeZoneAllocation(&memory.smalls, zone, ptr);
		return;
	}
}

void free(void *ptr)
{
	if (pthread_mutex_lock(&memory_mutex) != 0) {
		return;
	}
#ifdef ENABLE_DEBUG_VARIABLES
	if (memory.debug_variables.is_initialized == false) {
		setDebugVariables(&memory.debug_variables);
	}
#endif
	freeImplementation(ptr);
	if (pthread_mutex_unlock(&memory_mutex) != 0) {
		assert(!"pthread_mutex_unlock EPERM error");
	}
}
