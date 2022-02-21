#ifndef LIBFT_MALLOC_MEMORY_H
#define LIBFT_MALLOC_MEMORY_H

#include <pthread.h>

#include "libft_malloc/malloc_memory_list.h"
#include "libft_malloc/zones_list.h"
#include "libft_malloc/larges_list.h"

#define TINY_MAX_SIZE 256
#define SMALL_MAX_SIZE 4096

typedef struct memory_s
{
	malloc_memory_list_t *malloc_memory;
	zones_list_t *tinys;
	zones_list_t *smalls;
	larges_list_t *larges;
}	memory_t;

extern memory_t memory;
extern pthread_mutex_t memory_mutex;

#endif
