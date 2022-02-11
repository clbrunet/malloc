#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>

#include "libft_malloc/malloc_memory_list.h"
#include "libft_malloc/memory.h"
#include "libft_malloc/utils/math.h"

static malloc_memory_list_t *mallocMemoryList(size_t size)
{
	assert(size != 0);

	size = getNextMultiple(size + sizeof(malloc_memory_list_t), getpagesize());
	void *memory = mmap(NULL, size, PROT_WRITE | PROT_READ,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (memory == MAP_FAILED) {
		return NULL;
	}

	malloc_memory_list_t *malloc_memory = memory;
	malloc_memory->memory = memory;
	malloc_memory->unused = memory + sizeof(malloc_memory_list_t);
	malloc_memory->remaining_bytes_count = size - sizeof(malloc_memory_list_t);
	malloc_memory->next = NULL;
	return malloc_memory;
}

void *getMallocMemory(size_t size)
{
	assert(size != 0);

	malloc_memory_list_t **malloc_memory_list = &memory.malloc_memory;
	while (*malloc_memory_list != NULL
		&& (*malloc_memory_list)->remaining_bytes_count < size) {
		malloc_memory_list = &(*malloc_memory_list)->next;
	}
	if (*malloc_memory_list == NULL) {
		*malloc_memory_list = mallocMemoryList(size);
		if (*malloc_memory_list == NULL) {
			return NULL;
		}
	}
	malloc_memory_list_t *malloc_memory = *malloc_memory_list;

	void *ptr = malloc_memory->unused;
	malloc_memory->unused += size;
	malloc_memory->remaining_bytes_count -= size;
	return ptr;
}

void resetTopMallocMemory(void *ptr, size_t size)
{
	assert(ptr != NULL);
	assert(size != 0);

	malloc_memory_list_t *malloc_memory = memory.malloc_memory;
	while (malloc_memory != NULL) {
		if (ptr == malloc_memory->unused - size) {
			malloc_memory->unused -= size;
			malloc_memory->remaining_bytes_count += size;
			return;
		}
		malloc_memory = malloc_memory->next;
	}
}
