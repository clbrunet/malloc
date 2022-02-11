#include <unistd.h>
#include <sys/mman.h>
#include <assert.h>

#include "libft_malloc/larges_list.h"
#include "libft_malloc/malloc_memory_list.h"
#include "libft_malloc/utils/math.h"

larges_list_t *largesList(size_t size)
{
	assert(size != 0);

	larges_list_t *larges_list = getMallocMemory(sizeof(larges_list_t));
	if (larges_list == NULL) {
		return NULL;
	}
	if (largesListSetMemory(larges_list, size) == -1) {
		resetTopMallocMemory(larges_list, sizeof(larges_list_t));
		return NULL;
	}
	larges_list->next = NULL;
	return larges_list;
}

int largesListSetMemory(larges_list_t *large, size_t size)
{
	assert(large != NULL);
	assert(size != 0);

	size_t max_size = getNextMultiple(size, getpagesize());
	large->memory = mmap(NULL, max_size, PROT_WRITE | PROT_READ,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (large->memory == NULL) {
		large->max_size = 0;
		large->size = 0;
		return -1;
	}
	large->max_size = max_size;
	large->size = size;
	return 0;
}

int largesListUnsetMemory(larges_list_t *large)
{
	assert(large != NULL);

	int ret = munmap(large->memory, large->size);
	large->max_size = 0;
	large->size = 0;
	large->memory = NULL;
	return ret;
}

larges_list_t *largesListSearchPtr(larges_list_t *larges, void *ptr)
{
	assert(ptr != NULL);

	while (larges != NULL) {
		if (ptr == larges->memory) {
			return larges;
		}
		larges = larges->next;
	}
	return NULL;
}
