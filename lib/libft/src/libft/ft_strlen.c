#include <stddef.h>

size_t strlen(const char *s)
{
	const char *iter = s;

	while (*iter != '\0') {
		iter++;
	}
	return (size_t)(iter - s);
}
