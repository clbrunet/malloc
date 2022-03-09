#ifndef LIBFT_MALLOC_SHOW_ALLOC_STATS_H
#define LIBFT_MALLOC_SHOW_ALLOC_STATS_H

#include <stddef.h>

typedef struct zones_stats_s
{
	size_t total_map_size;
	size_t total_free_bytes_count;
}	zones_stats_t;

#endif
