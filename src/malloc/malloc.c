#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

void *malloc(size_t size)
{
	if (size == 0) {
		return NULL;
	}
	assert(false);
}
