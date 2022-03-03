#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>

#include "libft_malloc/zones.h"
#include "libft_malloc/block.h"
#include "libft_malloc/memory.h"
#include "libft_malloc/utils/math.h"
#include "libft_malloc/utils/print.h"

zones_t *zonesCreate(size_t allocation_max_size)
{
	assert(allocation_max_size != 0);

	size_t map_size = sizeof(zones_t) + 100 * (sizeof(block_t) + allocation_max_size);
	map_size = getNextMultiple(map_size, getpagesize());

	zones_t *zones = mmap(NULL, map_size, PROT_WRITE | PROT_READ,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (zones == MAP_FAILED) {
		return NULL;
	}

	zones->map_size = map_size;
	zones->leftmost_free_block = ZONE_START(zones);
	zones->leftmost_free_block->prev = NULL;
	zones->leftmost_free_block->is_free = true;
	zones->leftmost_free_block->size = map_size - (sizeof(zones_t) + sizeof(block_t));
	zones->next = NULL;
	return zones;
}

bool isPtrInZone(const zones_t *zone, const void *ptr)
{
	assert(zone != NULL);
	assert(ptr != NULL);

	if (ZONE_START(zone) <= ptr && ptr < ZONE_END(zone)) {
		return true;
	}
	return false;
}

zones_t *zonesSearchPtr( zones_t *zones, const void *ptr)
{
	assert(ptr != NULL);

	while (zones != NULL) {
		if (isPtrInZone(zones, ptr)) {
			return zones;
		}
		zones = zones->next;
	}
	return NULL;
}

void zonesDelete(zones_t **zones, zones_t *to_delete)
{
	assert(zones != NULL);
	assert(to_delete != NULL);

	zones_t *zone = *zones;
	if (zone == to_delete) {
		*zones = to_delete->next;
		munmap(to_delete, to_delete->map_size);
		return;
	}
	while (zone != NULL) {
		if (zone->next == to_delete) {
			zone->next = to_delete->next;
			munmap(to_delete, to_delete->map_size);
			return;
		}
		zone = zone->next;
	}
}
