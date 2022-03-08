#include <assert.h>

#include "libft_malloc/memory.h"
#include "libft_malloc/block.h"
#include "libft_malloc/utils/print.h"

static void showHistory(allocation_histories_t *histories)
{
	printStr(FG_YELLOW "Allocation history :\n" RESET);
	if (histories == NULL) {
		printStr("Nothing at the moment\n");
		return;
	}
	while (histories != NULL) {
		allocation_histories_entry_t *entries = ALLOCATION_HISTORY_START(histories);
		for (size_t i = 0; i < histories->next_entry_index; i++) {
			printNbr(entries[i].size);
			printStr(" bytes");
			if (entries[i].is_a_reallocation == true) {
				printStr(" (reallocation)");
			}
			printEndl();
		}
		histories = histories->next;
	}
}

static void showAllocationHexDump(unsigned char *allocation_address, size_t size)
{
	assert(allocation_address != NULL);
	assert(size != 0);

	const size_t offset_minimum_length = 7;

	printHexMinimumLength(0, offset_minimum_length, false);
	printChar(' ');
	printHexMinimumLength(*allocation_address, 2, false);
	allocation_address++;
	size_t i = 1;
	while (i < size) {
		if (i % 16 == 0) {
			printEndl();
			printHexMinimumLength(i, offset_minimum_length, false);
			printChar(' ');
		} else if (i % 2 == 0) {
			printChar(' ');
		}
		printHexMinimumLength(*allocation_address, 2, false);
		allocation_address++;
		i++;
	}
	printEndl();
	printHexMinimumLength(i, offset_minimum_length, false);
	printEndl();
}

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
	showAllocationHexDump((unsigned char *)allocation_address, size);
	return size;
}

static size_t showZones(zones_t *zones, const char *name)
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

static size_t showLarges(larges_t *larges)
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

static void showCurrentAllocations()
{
	size_t total = 0;
	printStr(FG_CYAN "Current allocations :\n" RESET);
	total += showZones(memory.tinys, "TINY");
	total += showZones(memory.smalls, "SMALL");
	total += showLarges(memory.larges);
	printStr(FG_BLUE "Total : ");
	printNbr(total);
	printStr(" bytes\n" RESET);
}

void show_alloc_mem_ex()
{
	if (pthread_mutex_lock(&memory_mutex) != 0) {
		return;
	}
	showHistory(memory.histories);
	printEndl();
	showCurrentAllocations();
	if (pthread_mutex_unlock(&memory_mutex) != 0) {
		assert(!"pthread_mutex_unlock EPERM error");
	}
}
