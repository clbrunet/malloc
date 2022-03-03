#ifndef LIBFT_MALLOC_MALLOC_H
#define LIBFT_MALLOC_MALLOC_H

#include <stddef.h>

typedef enum allocation_history_action_e allocation_history_action_t;

void *mallocImplementation(size_t size, allocation_history_action_t allocation_history_action);

#endif
