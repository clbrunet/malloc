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
	size_t page_size = getpagesize();
	zones_stats.total_map_size_pages = zones_stats.total_map_size / page_size;
	zones_stats.total_free_bytes_count_pages = zones_stats.total_free_bytes_count / page_size;
	return zones_stats;
}

static larges_stats_t getLargesStats(const larges_t *larges)
{
	larges_stats_t larges_stats = {};
	while (larges != NULL) {
		larges_stats.total_map_size += sizeof(larges_t) + larges->size;
		larges = larges->next;
	}
	larges_stats.total_map_size_pages = larges_stats.total_map_size / getpagesize();
	return larges_stats;
}

static zones_stats_t showZonesStats(const zones_t *zones, const char *name)
{
	assert(name != NULL);

	zones_stats_t zones_stats = getZonesStats(zones);
	printStr(name);
	printStr(" : \n  Used : ");
	printNbr(zones_stats.total_map_size - zones_stats.total_free_bytes_count);
	printStr("/");
	printNbr(zones_stats.total_map_size);
	printStr(" bytes (~");
	printNbr(zones_stats.total_map_size_pages - zones_stats.total_free_bytes_count_pages);
	printStr("/");
	printNbr(zones_stats.total_map_size_pages);
	printStr(" pages)\n  Free : ");
	printNbr(zones_stats.total_free_bytes_count);
	printStr("/");
	printNbr(zones_stats.total_map_size);
	printStr(" bytes (~");
	printNbr(zones_stats.total_free_bytes_count_pages);
	printStr("/");
	printNbr(zones_stats.total_map_size_pages);
	printStr(" pages)\n");
	return zones_stats;
}

static larges_stats_t showLargesStats(const larges_t *larges)
{
	larges_stats_t larges_stats = getLargesStats(larges);
	printStr("Larges :\n  Used : ");
	printNbr(larges_stats.total_map_size);
	printStr(" bytes (");
	printNbr(larges_stats.total_map_size_pages);
	printStr(" pages)\n");
	return larges_stats;
}

void show_alloc_stats()
{
	if (pthread_mutex_lock(&memory_mutex) != 0) {
		return;
	}
	zones_stats_t tinies_stats = showZonesStats(memory.tinies, "Tinies");
	zones_stats_t smalls_stats = showZonesStats(memory.smalls, "Smalls");
	larges_stats_t larges_stats = showLargesStats(memory.larges);

	size_t total_map_size = tinies_stats.total_map_size + smalls_stats.total_map_size
		+ larges_stats.total_map_size;
	size_t total_free_bytes = tinies_stats.total_free_bytes_count + smalls_stats.total_free_bytes_count;
	size_t page_size = getpagesize();
	size_t total_map_size_pages = total_map_size / page_size;
	size_t total_free_bytes_pages = total_free_bytes / page_size;

	printStr("Total :\n  Used : ");
	printNbr(total_map_size - total_free_bytes);
	printStr("/");
	printNbr(total_map_size);
	printStr(" bytes (~");
	printNbr(total_map_size_pages - total_free_bytes_pages);
	printStr("/");
	printNbr(total_map_size_pages);
	printStr(" pages)\n  Free : ");
	printNbr(total_free_bytes);
	printStr("/");
	printNbr(total_map_size);
	printStr(" bytes (~");
	printNbr(total_free_bytes_pages);
	printStr("/");
	printNbr(total_map_size_pages);
	printStr(" pages)\n");

	if (pthread_mutex_unlock(&memory_mutex) != 0) {
		assert(!"pthread_mutex_unlock EPERM error");
	}
}
