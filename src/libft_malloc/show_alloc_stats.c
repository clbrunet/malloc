#include <assert.h>
#include <unistd.h>

#include "libft_malloc/show_alloc_stats.h"
#include "libft_malloc/memory.h"
#include "libft_malloc/block.h"
#include "libft_malloc/utils/print.h"

static zones_stats_t getZonesStats(const zones_t *zones)
{
	zones_stats_t zones_stats = {};
	while (zones != NULL) {
		zones_stats.total_map_size += zones->map_size;
		block_t *block = ZONE_START(zones);
		void *zone_end = ZONE_END(zones);
		while ((void *)block < zone_end) {
			if (block->is_free == true) {
				zones_stats.total_free_bytes_count += block->size;
			}
			block = BLOCK_NEXT(block);
		}
		zones = zones->next;
	}
	return zones_stats;
}

static void showZonesStats(const zones_t *zones, const char *name)
{
	assert(name != NULL);

	zones_stats_t zones_stats;
	zones_stats = getZonesStats(zones);
	size_t total_map_size_pages = zones_stats.total_map_size / getpagesize();
	size_t total_free_bytes_count_pages = zones_stats.total_free_bytes_count / getpagesize();

	printStr(name);
	printStr(" : \n  Used : ");
	printNbr(zones_stats.total_map_size - zones_stats.total_free_bytes_count);
	printStr("/");
	printNbr(zones_stats.total_map_size);
	printStr(" bytes (~");
	printNbr(total_map_size_pages - total_free_bytes_count_pages);
	printStr("/");
	printNbr(total_map_size_pages);
	printStr(" pages)\n  Free : ");
	printNbr(zones_stats.total_free_bytes_count);
	printStr("/");
	printNbr(zones_stats.total_map_size);
	printStr(" bytes (~");
	printNbr(total_free_bytes_count_pages);
	printStr("/");
	printNbr(total_map_size_pages);
	printStr(" pages)\n");
}

static void showLargesStats(larges_t *larges)
{
	size_t total = 0;
	while (larges != NULL) {
		total += sizeof(larges_t) + larges->size;
		larges = larges->next;
	}
	printStr("Larges :\n  Used : ");
	printNbr(total);
	printStr(" bytes (");
	printNbr(total / getpagesize());
	printStr(" pages)\n");
}

void show_alloc_stats()
{
	if (pthread_mutex_lock(&memory_mutex) != 0) {
		return;
	}
	showZonesStats(memory.tinies, "Tinies");
	showZonesStats(memory.smalls, "Smalls");
	showLargesStats(memory.larges);
	if (pthread_mutex_unlock(&memory_mutex) != 0) {
		assert(!"pthread_mutex_unlock EPERM error");
	}
}
