#include <stddef.h>
#include <assert.h>

size_t getStringLength(const char *str)
{
	assert(str != NULL);

	const char *iter = str;
	while (*iter != '\0') {
		iter++;
	}
	return iter - str;
}

void *memoryCopy(void *dst, const void *src, size_t n)
{
	assert(dst != NULL);
	assert(src != NULL);

	char *s_dst = dst;
	const char *s_src = src;
	while (n > 0) {
		*s_dst = *s_src;
		s_dst++;
		s_src++;
		n--;
	}
	return (dst);
}
