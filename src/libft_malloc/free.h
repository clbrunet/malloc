#ifndef LIBFT_MALLOC_FREE_H
#define LIBFT_MALLOC_FREE_H

#include "libft_malloc/zones_list.h"

void freeZoneAllocation(zone_allocation_t *zone_allocation);
void freeImplementation(void *ptr);

#endif
