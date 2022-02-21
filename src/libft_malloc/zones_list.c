#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>

#include "libft_malloc/zones_list.h"
#include "libft_malloc/memory.h"
#include "libft_malloc/utils/math.h"
#include "libft_malloc/utils/print.h"

zones_list_t *zonesList(size_t allocation_max_size)
{
	assert(allocation_max_size != 0);

	zones_list_t *zones_list = getMallocMemory(sizeof(zones_list_t));
	if (zones_list == NULL) {
		return NULL;
	}
	size_t zone_size = getNextMultiple(allocation_max_size * 100, getpagesize());

	zones_list->allocations_count = zone_size / allocation_max_size;
	zones_list->min_free_allocation_index = 0;
	zones_list->sizes = getMallocMemory(zones_list->allocations_count * sizeof(size_t));
	if (zones_list->sizes == NULL) {
		resetTopMallocMemory(zones_list, sizeof(zones_list_t));
		return NULL;
	}
	size_t *sizes_iter = zones_list->sizes;
	for (size_t i = 0; i < zones_list->allocations_count; i++) {
		*sizes_iter = 0;
		sizes_iter++;
	}
	zones_list->memory = mmap(NULL, zone_size, PROT_WRITE | PROT_READ,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (zones_list->memory == NULL) {
		resetTopMallocMemory(zones_list->sizes, zones_list->allocations_count * sizeof(size_t));
		resetTopMallocMemory(zones_list, sizeof(zones_list_t));
		return NULL;
	}
	zones_list->min_free_allocation_index = 0;
	zones_list->next = NULL;
	return zones_list;
}

zone_allocation_t zonesListSearchPtr(zones_list_t *zone, size_t allocation_max_size, void *ptr)
{
	assert(allocation_max_size != 0);
	assert(ptr != NULL);

	while (zone != NULL) {
		if (zone->memory <= ptr
			&& ptr < zone->memory + (zone->allocations_count * allocation_max_size)) {
			return (zone_allocation_t){
				.zone = zone,
				.index = (ptr - zone->memory) / allocation_max_size,
			};
		}
		zone = zone->next;
	}
	return (zone_allocation_t){
		.zone = NULL,
		.index = (size_t)-1,
	};
}

