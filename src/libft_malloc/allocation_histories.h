#ifndef LIBFT_MALLOC_ALLOCATION_HISTORIES_H
#define LIBFT_MALLOC_ALLOCATION_HISTORIES_H

#include <stddef.h>
#include <stdbool.h>

typedef enum allocation_history_action_e
{
	AddHistoryEntry,
	DontAddHistoryEntry,
}	allocation_history_action_t;

typedef struct allocation_histories_entry_s allocation_histories_entry_t;
struct allocation_histories_entry_s
{
	size_t size;
	bool is_a_reallocation;
};

typedef struct allocation_histories_s allocation_histories_t;
struct allocation_histories_s
{
	size_t map_size;
	size_t max_entry_index;
	size_t next_entry_index;
	allocation_histories_t *next;
};

#define ALLOCATION_HISTORY_START(allocation_history) \
	((void *)(allocation_history) + sizeof(allocation_histories_t))

allocation_histories_t *allocationHistoriesCreate();
int allocationHistoriesAddEntry(allocation_histories_t **histories,
		allocation_histories_entry_t entry);
bool isAllocationHistoryActionValid(allocation_history_action_t allocation_history_action);

#endif
