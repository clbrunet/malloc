#ifndef LIBFT_MALLOC_ZONES_H
#define LIBFT_MALLOC_ZONES_H

#include <stddef.h>
#include <stdbool.h>

typedef struct block_s block_t;

typedef struct zones_s zones_t;
struct zones_s
{
	size_t map_size;
	block_t *leftmost_free_block;
	size_t blocks_used_count;
	zones_t *next;
};

#define ZONE_START(zone) ((void *)(zone) + sizeof(zones_t))
#define ZONE_END(zone) ((void *)(zone) + (zone)->map_size)

zones_t *zonesCreate(size_t allocation_max_size);
bool isPtrInZone(const zones_t *zone, const void *ptr);
zones_t *zonesSearchPtr( zones_t *zones, const void *ptr);
void zonesDelete(zones_t **zones, zones_t *to_delete);
void showZones(zones_t *zones, const char *name);

#endif
