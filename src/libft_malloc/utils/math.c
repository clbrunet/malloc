#include <assert.h>

#include "libft_malloc/utils/math.h"

size_t min(size_t a, size_t b)
{
	if (a < b) {
		return a;
	}
	return b;
}

size_t getNextMultiple(size_t n, size_t submultiple)
{
	assert(submultiple != 0);

	if (n < submultiple) {
		return submultiple;
	}
	size_t remainder = n % submultiple;
	if (remainder != 0) {
		return n - remainder + submultiple;
	} else {
		return n;
	}
}
