#include <assert.h>

#include "libft_malloc/memory.h"
#include "libft_malloc/utils/print.h"

static size_t show_zones(zones_list_t *zones, const char *name, size_t allocation_max_size)
{
	assert(name != NULL);
	assert(allocation_max_size != 0);

	size_t total = 0;
	while (zones != NULL) {
		printStr(name);
		printStr(" : Ox");
		printHex((long long)zones->memory);
		printEndl();
		for (size_t i = 0; i < zones->allocations_count; i++) {
			if (zones->sizes[i] != 0) {
				long long allocation_address = (long long)zones->memory + (i * allocation_max_size);
				size_t size = zones->sizes[i];
				printStr("Ox");
				printHex(allocation_address);
				printStr(" - Ox");
				printHex(allocation_address + size);
				printStr(" : ");
				printNbr(size);
				printStr(" bytes\n");
				total += size;
			}
		}
		zones = zones->next;
	}
	return total;
}

static size_t show_larges(larges_list_t *larges)
{
	size_t total = 0;
	while (larges != NULL) {
		if (larges->memory != NULL) {
			long long address = (long long)larges->memory;
			size_t size = larges->size;
			printStr("LARGE : Ox");
			printHex(address);
			printEndl();
			printStr("Ox");
			printHex(address);
			printStr(" - Ox");
			printHex(address + size);
			printStr(" : ");
			printNbr(size);
			printStr(" bytes\n");
			total += size;
		}
		larges = larges->next;
	}
	return total;
}

void show_alloc_mem()
{
	size_t total = 0;
	total += show_zones(memory.tinys, "TINY", TINY_MAX_SIZE);
	total += show_zones(memory.smalls, "SMALL", SMALL_MAX_SIZE);
	total += show_larges(memory.larges);
	printStr("Total : ");
	printNbr(total);
	printStr(" octets\n");
}
