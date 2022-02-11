#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "libft_malloc/larges_list.h"
#include "libft_malloc/utils/print.h"
#include "libft_malloc/memory.h"
#include "libft_malloc/zones_list.h"

memory_t memory = {
	.malloc_memory = NULL,
	.tinys = NULL,
	.smalls = NULL,
	.larges = NULL,
};

static void *getZoneAllocation(zones_list_t **zones, size_t allocation_max_size, size_t size)
{
	assert(zones != NULL);
	assert(allocation_max_size != 0);
	assert(size != 0);

	while (*zones != NULL && (*zones)->min_free_allocation_index >= (*zones)->allocations_count) {
		zones = &(*zones)->next;
	}
	if (*zones == NULL) {
		*zones = zonesList(allocation_max_size);
		if (*zones == NULL) {
			return NULL;
		}
	}
	zones_list_t *zone = *zones;
	while (zone->sizes[zone->min_free_allocation_index] != 0) {
		zone->min_free_allocation_index++;
		assert(zone->min_free_allocation_index < zone->allocations_count);
	}
	zone->sizes[zone->min_free_allocation_index] = size;
	zone->min_free_allocation_index++;
	return zone->memory + ((zone->min_free_allocation_index - 1) * allocation_max_size);
}

static void *getLargeAllocation(larges_list_t **larges, size_t size)
{
	assert(larges != NULL);
	assert(size != 0);

	while (*larges != NULL && (*larges)->memory != NULL) {
		larges = &(*larges)->next;
	}
	if (*larges != NULL) {
		if (largesListSetMemory(*larges, size) == -1) {
			return NULL;
		}
	} else {
		*larges = largesList(size);
		if (*larges == NULL) {
			return NULL;
		}
	}
	return (*larges)->memory;
}

void *malloc(size_t size)
{
	if (size == 0) {
		return NULL;
	}

	void *ptr = NULL;
	if (size <= TINY_MAX_SIZE) {
		ptr = getZoneAllocation(&memory.tinys, TINY_MAX_SIZE, size);
	} else if (size <= SMALL_MAX_SIZE) {
		ptr = getZoneAllocation(&memory.smalls, SMALL_MAX_SIZE, size);
	} else {
		ptr = getLargeAllocation(&memory.larges, size);
	}
	return ptr;
}
