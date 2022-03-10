#include <stdbool.h>
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

void *memorySet(void *dst, unsigned char c, size_t n)
{
	assert(dst != NULL);

	char *s_dst = dst;
	while (n > 0) {
		*s_dst = c;
		s_dst++;
		n--;
	}
	return (dst);
}
