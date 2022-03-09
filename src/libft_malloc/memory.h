#ifndef LIBFT_MALLOC_MEMORY_H
#define LIBFT_MALLOC_MEMORY_H

#include <pthread.h>

#include "libft_malloc/zones.h"
#include "libft_malloc/larges.h"
#include "libft_malloc/allocation_histories.h"

#define TINY_MAX_SIZE 256
#define SMALL_MAX_SIZE 4096

typedef struct memory_s
{
	zones_t *tinies;
	zones_t *smalls;
	larges_t *larges;
	allocation_histories_t *histories;
}	memory_t;

extern memory_t memory;
extern pthread_mutex_t memory_mutex;

#endif
