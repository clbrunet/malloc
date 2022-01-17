#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

void free(void *ptr)
{
	if (ptr == NULL) {
		return;
	}
	assert(false);
}
