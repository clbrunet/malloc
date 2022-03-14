#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>

#include "libft_malloc/allocation_histories.h"
#include "libft_malloc/memory.h"
#include "libft_malloc/utils/math.h"
#include "libft_malloc/utils/print.h"

allocation_histories_t *allocationHistoriesCreate()
{
	size_t map_size = sizeof(allocation_histories_t) + 100 * sizeof(allocation_histories_entry_t);
	map_size = getNextMultiple(map_size, getpagesize());

	allocation_histories_t *allocation_histories = mmap(NULL, map_size, PROT_WRITE | PROT_READ,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (allocation_histories == MAP_FAILED) {
		return NULL;
	}

	allocation_histories->map_size = map_size;
	allocation_histories->max_entry_index = ((map_size - sizeof(allocation_histories_t))
		/ sizeof(allocation_histories_entry_t)) - 1;
	allocation_histories->next_entry_index = 0;
	allocation_histories->next = NULL;
	return allocation_histories;
}

int allocationHistoriesAddEntry(allocation_histories_t **histories,
		allocation_histories_entry_t entry)
{
	assert(histories != NULL);

	while (*histories != NULL) {
		allocation_histories_t *history = *histories;
		if (history->next_entry_index <= history->max_entry_index) {
			allocation_histories_entry_t *entries = (void *)history + sizeof(allocation_histories_t);
			entries[history->next_entry_index] = entry;
			history->next_entry_index++;
			return 0;
		}
		histories = &history->next;
	}

	*histories = allocationHistoriesCreate();
	if (*histories == NULL) {
		return -1;
	}
	allocation_histories_t *history = *histories;
	allocation_histories_entry_t *entries = (void *)history + sizeof(allocation_histories_t);
	entries[0] = entry;
	history->next_entry_index = 1;
	return 0;
}

bool isAllocationHistoryActionValid(allocation_history_action_t allocation_history_action)
{
	switch (allocation_history_action) {
		case AddHistoryEntry:
			return true;
		case DontAddHistoryEntry:
			return true;
	}
	return false;
}
