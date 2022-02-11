#ifndef LIBFT_MALLOC_ZONES_LIST_H
#define LIBFT_MALLOC_ZONES_LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct zones_list_s zones_list_t;
struct zones_list_s
{
	size_t allocations_count;
	size_t min_free_allocation_index;
	size_t *sizes;
	void *memory;
	zones_list_t *next;
};

typedef struct zone_allocation_s
{
	zones_list_t *zone;
	size_t index;
}	zone_allocation_t;

zones_list_t *zonesList(size_t allocation_max_size);
zone_allocation_t zonesListSearchPtr(zones_list_t *zone, size_t allocation_max_size, void *ptr);

#endif
