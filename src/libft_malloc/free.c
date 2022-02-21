#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "libft_malloc/memory.h"
#include "libft_malloc/utils/print.h"

void freeZoneAllocation(zone_allocation_t *zone_allocation)
{
	assert(zone_allocation != NULL
			&& zone_allocation->zone != NULL
			&& zone_allocation->index != (size_t)-1);
	zone_allocation->zone->sizes[zone_allocation->index] = 0;
	if (zone_allocation->index < zone_allocation->zone->min_free_allocation_index) {
		zone_allocation->zone->min_free_allocation_index = zone_allocation->index;
	}
}

void free(void *ptr)
{
	if (ptr == NULL) {
		return;
	}

	pthread_mutex_lock(&memory_mutex);
	larges_list_t *large = largesListSearchPtr(memory.larges, ptr);
	if (large != NULL) {
		largesListUnsetMemory(large);
		pthread_mutex_unlock(&memory_mutex);
		return;
	}
	zone_allocation_t zone_allocation;
	zone_allocation = zonesListSearchPtr(memory.tinys, TINY_MAX_SIZE, ptr);
	if (zone_allocation.zone != NULL) {
		freeZoneAllocation(&zone_allocation);
		pthread_mutex_unlock(&memory_mutex);
		return;
	}
	zone_allocation = zonesListSearchPtr(memory.smalls, SMALL_MAX_SIZE, ptr);
	if (zone_allocation.zone != NULL) {
		freeZoneAllocation(&zone_allocation);
		pthread_mutex_unlock(&memory_mutex);
		return;
	}
}
