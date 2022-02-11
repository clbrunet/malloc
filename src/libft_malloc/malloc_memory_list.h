#ifndef LIBFT_MALLOC_MALLOC_MEMORY_LIST_H
#define LIBFT_MALLOC_MALLOC_MEMORY_LIST_H

#include <stddef.h>

typedef struct malloc_memory_list_s malloc_memory_list_t;
struct malloc_memory_list_s
{
	void *memory;
	void *unused;
	size_t remaining_bytes_count;
	malloc_memory_list_t *next;
};

void *getMallocMemory(size_t size);
void resetTopMallocMemory(void* ptr, size_t size);

#endif
