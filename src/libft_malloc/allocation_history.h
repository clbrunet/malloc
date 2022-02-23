#ifndef LIBFT_MALLOC_ALLOCATION_HISTORY_H
#define LIBFT_MALLOC_ALLOCATION_HISTORY_H

#include <stddef.h>
#include <stdbool.h>

typedef struct allocation_history_s allocation_history_t;
struct allocation_history_s
{
	size_t size;
	bool is_a_reallocation;
	allocation_history_t *next;
};

allocation_history_t *allocationHistory(size_t size, bool is_a_reallocation);
allocation_history_t *allocationHistoryGetBack(allocation_history_t *history);
void allocationHistoryPushBack(allocation_history_t **history, allocation_history_t *new);

#endif
