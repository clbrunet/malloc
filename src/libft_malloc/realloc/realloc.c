#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "libft_malloc.h"

void *realloc(void *ptr, size_t size)
{
	if (ptr == NULL) {
		return malloc(size);
	}
	if (size == 0) {
		free(ptr);
		return malloc(0);
	}
	assert(false);
}
