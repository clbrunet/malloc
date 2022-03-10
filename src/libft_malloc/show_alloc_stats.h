#ifndef LIBFT_MALLOC_SHOW_ALLOC_STATS_H
#define LIBFT_MALLOC_SHOW_ALLOC_STATS_H

#include <stddef.h>

typedef struct zones_stats_s
{
	size_t total_map_size;
	size_t total_map_size_pages;
	size_t total_free_bytes_count;
	size_t total_free_bytes_count_pages;
} zones_stats_t;

typedef struct larges_stats_s
{
	size_t total_map_size;
	size_t total_map_size_pages;
} larges_stats_t;

#endif
