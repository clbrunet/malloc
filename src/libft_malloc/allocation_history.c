#include <assert.h>

#include "libft_malloc/allocation_history.h"
#include "libft_malloc/memory.h"

allocation_history_t *allocationHistory(size_t size, bool is_a_reallocation)
{
	allocation_history_t *allocation_history = getMallocMemory(sizeof(allocation_history_t));
	if (allocation_history == NULL) {
		return NULL;
	}
	allocation_history->size = size;
	allocation_history->is_a_reallocation = is_a_reallocation;
	allocation_history->next = NULL;
	return allocation_history;
}

allocation_history_t *allocationHistoryGetBack(allocation_history_t *history)
{
	assert(history != NULL);

	while (history->next != NULL) {
		history = history->next;
	}
	return history;
}

void allocationHistoryPushBack(allocation_history_t **history, allocation_history_t *new)
{
	assert(new != NULL);

	if (*history == NULL) {
		*history = new;
		return;
	}

	allocation_history_t *back = allocationHistoryGetBack(*history);
	back->next = new;
}
