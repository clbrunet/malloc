#include <unistd.h>
#include <sys/mman.h>
#include <assert.h>

#include "libft_malloc/larges.h"
#include "libft_malloc/memory.h"
#include "libft_malloc/utils/math.h"
#include "libft_malloc/utils/print.h"

larges_t *largesCreate(size_t size)
{
	assert(size != 0);

	size_t map_size = sizeof(larges_t) + size;
	map_size = getNextMultiple(map_size, getpagesize());

#ifdef ENABLE_DEBUG_VARIABLES
	if (memory.debug_variables.max_bytes != 0) {
		if (memory.used_bytes_count + map_size - sizeof(larges_t)
			> memory.debug_variables.max_bytes) {
			return NULL;
		}
	}
#endif
	larges_t *larges = mmap(NULL, map_size, PROT_WRITE | PROT_READ,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (larges == MAP_FAILED) {
		return NULL;
	}
	larges->size = map_size - sizeof(larges_t);
	larges->next = NULL;
#ifdef ENABLE_DEBUG_VARIABLES
	memory.used_bytes_count += larges->size;
#endif
	return larges;
}

larges_t *largesSearchPtr(larges_t *larges, void *ptr)
{
	assert(ptr != NULL);

	while (larges != NULL) {
		if (LARGE_START(larges) == ptr) {
			return larges;
		}
		larges = larges->next;
	}
	return NULL;
}

void largesDelete(larges_t **larges, larges_t *to_delete)
{
	assert(larges != NULL);
	assert(to_delete != NULL);

	larges_t *large = *larges;
	if (large == to_delete) {
		*larges = to_delete->next;
#ifdef ENABLE_DEBUG_VARIABLES
		memory.used_bytes_count -= to_delete->size;
#endif
		munmap(to_delete, sizeof(larges_t) + to_delete->size);
		return;
	}
	while (large != NULL) {
		if (large->next == to_delete) {
			large->next = to_delete->next;
#ifdef ENABLE_DEBUG_VARIABLES
		memory.used_bytes_count -= to_delete->size;
#endif
			munmap(to_delete, sizeof(larges_t) + to_delete->size);
			return;
		}
		large = large->next;
	}
}

void showLarges(larges_t *larges)
{
	printStr("Larges : ");
	if (larges == NULL) {
		printStr("nothing\n");
		return;
	}
	printNbr(larges->size);
	larges = larges->next;
	while (larges != NULL) {
		printStr(" -> ");
		printNbr(larges->size);
		larges = larges->next;
	}
	printEndl();
}
