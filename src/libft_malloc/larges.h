#ifndef LIBFT_MALLOC_LARGES_H
#define LIBFT_MALLOC_LARGES_H

#include <stddef.h>

typedef struct larges_s larges_t;
struct larges_s
{
	size_t size;
	larges_t *next;
};

#define LARGE_START(large) ((void *)(large) + sizeof(larges_t))

larges_t *largesCreate(size_t size);
larges_t *largesSearchPtr(larges_t *larges, void *ptr);
void largesDelete(larges_t **larges, larges_t *to_delete);

#endif
