#include <assert.h>

#include "libft_malloc/memory.h"
#include "libft_malloc/utils/print.h"

static void showAllocationHexDump(unsigned char *allocation_address, size_t size)
{
	if (size == 0) {
		return;
	}

	printHexMinimumLength(*allocation_address, 2);
	allocation_address++;
	for (size_t i = 1; i < size; i++) {
		if (i % 32 == 0) {
			printChar('\n');
		} else if (i % 2 == 0) {
			printChar(' ');
		}
		printHexMinimumLength(*allocation_address, 2);
		allocation_address++;
	}
	printEndl();
}

static size_t showAllocation(long allocation_address, size_t size)
{
	printStr("Ox");
	printHex(allocation_address);
	printStr(" - Ox");
	printHex(allocation_address + size);
	printStr(" : ");
	printNbr(size);
	printStr(" bytes :\n");
	showAllocationHexDump((unsigned char *)allocation_address, size);
	return size;
}

static size_t showZones(zones_list_t *zones, const char *name, size_t allocation_max_size)
{
	assert(name != NULL);
	assert(allocation_max_size != 0);

	size_t total = 0;
	while (zones != NULL) {
		long address = (long)zones->memory;
		printStr(name);
		printStr(" : Ox");
		printHex(address);
		printEndl();
		for (size_t i = 0; i < zones->allocations_count; i++) {
			if (zones->sizes[i] != 0) {
				total += showAllocation(address, zones->sizes[i]);
			}
			address += allocation_max_size;
		}
		zones = zones->next;
	}
	return total;
}

static size_t showLarges(larges_list_t *larges)
{
	size_t total = 0;
	while (larges != NULL) {
		if (larges->memory != NULL) {
			long address = (long)larges->memory;
			printStr("LARGE : Ox");
			printHex(address);
			printEndl();
			total += showAllocation(address, larges->size);
		}
		larges = larges->next;
	}
	return total;
}

void show_alloc_mem_ex()
{
	size_t total = 0;
	pthread_mutex_lock(&memory_mutex);
	total += showZones(memory.tinys, "TINY", TINY_MAX_SIZE);
	total += showZones(memory.smalls, "SMALL", SMALL_MAX_SIZE);
	total += showLarges(memory.larges);
	printStr("Total : ");
	printNbr(total);
	printStr(" bytes\n");
	pthread_mutex_unlock(&memory_mutex);
}
