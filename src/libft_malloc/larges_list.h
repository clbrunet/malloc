#ifndef LIBFT_MALLOC_LARGES_LIST_H
#define LIBFT_MALLOC_LARGES_LIST_H

#include <stddef.h>

typedef struct larges_list_s larges_list_t;
struct larges_list_s
{
	size_t max_size;
	size_t size;
	void *memory;
	larges_list_t *next;
};

larges_list_t *largesList(size_t size);
int largesListSetMemory(larges_list_t *large, size_t size);
int largesListUnsetMemory(larges_list_t *large);
larges_list_t *largesListSearchPtr(larges_list_t *larges, void *ptr);

#endif
