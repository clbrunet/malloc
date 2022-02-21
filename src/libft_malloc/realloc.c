#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "libft_malloc/larges_list.h"
#include "libft_malloc/malloc.h"
#include "libft_malloc/free.h"
#include "libft_malloc/memory.h"
#include "libft_malloc/utils/string.h"

static void *reallocLarge(larges_list_t *large, void *ptr, size_t size)
{
	assert(large != NULL);
	assert(ptr != NULL);
	assert(size != 0);

	if (size <= large->max_size) {
		large->size = size;
		return ptr;
	}

	void *new = malloc(size);
	if (new == NULL) {
		return NULL;
	}
	size_t min;
	if (size < large->size) {
		min = size;
	} else {
		min = large->size;
	}
	memoryCopy(new, ptr, min);
	largesListUnsetMemory(large);
	return new;
}

static void *reallocZoneAllocation(zone_allocation_t *zone_allocation, size_t allocation_max_size,
		void *ptr, size_t size)
{
	assert(zone_allocation != NULL
			&& zone_allocation->zone != NULL
			&& zone_allocation->index != (size_t)-1);
	assert(ptr != NULL);
	assert(size != 0);

	if (size <= allocation_max_size) {
		zone_allocation->zone->sizes[zone_allocation->index] = size;
		return ptr;
	}

	void *new = malloc(size);
	if (new == NULL) {
		return NULL;
	}
	size_t old_size = zone_allocation->zone->sizes[zone_allocation->index];
	size_t min;
	if (size < old_size) {
		min = size;
	} else {
		min = old_size;
	}
	memoryCopy(new, ptr, min);
	freeZoneAllocation(zone_allocation);
	return new;
}

static void *handleReallocErrors(void *ptr)
{
	if (ptr == NULL) {
		errno = ENOMEM;
	}
	return ptr;
}

void *realloc(void *ptr, size_t size)
{
	if (ptr == NULL) {
		return malloc(size);
	}
	if (size == 0) {
		free(ptr);
		return malloc(0);
	}

	pthread_mutex_lock(&memory_mutex);
	larges_list_t *large = largesListSearchPtr(memory.larges, ptr);
	if (large != NULL) {
		ptr = handleReallocErrors(reallocLarge(large, ptr, size));
		pthread_mutex_unlock(&memory_mutex);
		return ptr;
	}
	zone_allocation_t zone_allocation;
	zone_allocation = zonesListSearchPtr(memory.tinys, TINY_MAX_SIZE, ptr);
	if (zone_allocation.zone != NULL) {
		ptr = handleReallocErrors(reallocZoneAllocation(&zone_allocation,
					TINY_MAX_SIZE, ptr, size));
		pthread_mutex_unlock(&memory_mutex);
		return ptr;
	}
	zone_allocation = zonesListSearchPtr(memory.smalls, SMALL_MAX_SIZE, ptr);
	if (zone_allocation.zone != NULL) {
		ptr = handleReallocErrors(reallocZoneAllocation(&zone_allocation,
					SMALL_MAX_SIZE, ptr, size));
		pthread_mutex_unlock(&memory_mutex);
		return ptr;
	}
	return NULL;
}
