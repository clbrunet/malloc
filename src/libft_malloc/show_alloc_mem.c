#include <assert.h>

#include "libft_malloc/memory.h"
#include "libft_malloc/block.h"
#include "libft_malloc/utils/print.h"

static size_t showAllocation(long allocation_address, size_t size)
{
	assert(allocation_address != 0);
	assert(size != 0);

	printHex(allocation_address, true);
	printStr(" - ");
	printHex(allocation_address + size, true);
	printStr(" : ");
	printNbr(size);
	printStr(" bytes\n");
	return size;
}

static size_t showZones(const zones_t *zones, const char *name)
{
	assert(name != NULL);

	size_t total = 0;
	while (zones != NULL) {
		printStr(name);
		printStr(" : ");
		printHex((long long)zones, true);
		printEndl();
		block_t *block = ZONE_START(zones);
		void *zone_end = ZONE_END(zones);
		while ((void *)block < zone_end) {
			if (block->is_free == false) {
				total += showAllocation((long)BLOCK_START(block), block->size);
			}
			block = BLOCK_NEXT(block);
		}
		zones = zones->next;
	}
	return total;
}

static size_t showLarges(const larges_t *larges)
{
	size_t total = 0;
	while (larges != NULL) {
		printStr("LARGE : ");
		printHex((long long)larges, true);
		printEndl();
		total += showAllocation((long)LARGE_START(larges), larges->size);
		larges = larges->next;
	}
	return total;
}

void show_alloc_mem()
{
	if (pthread_mutex_lock(&memory_mutex) != 0) {
		return;
	}
	size_t total = 0;
	total += showZones(memory.tinies, "TINY");
	total += showZones(memory.smalls, "SMALL");
	total += showLarges(memory.larges);
	printStr("Total : ");
	printNbr(total);
	printStr(" bytes\n");
	if (pthread_mutex_unlock(&memory_mutex) != 0) {
		assert(!"pthread_mutex_unlock EPERM error");
	}
}
