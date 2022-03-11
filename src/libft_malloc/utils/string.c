#include <stdbool.h>
#include <limits.h>
#include <stddef.h>
#include <assert.h>

#include "libft_malloc/utils/char_type.h"

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

unsigned char stringToUChar(const char *str)
{
	unsigned char n = 0;
	while (isDigit(*str) == true && n <= (UCHAR_MAX - (*str - '0')) / 10) {
		n = n * 10 + (*str - '0');
		str++;
	}
	if (*str != '\0') {
		return 0;
	}
	return n;
}

unsigned long stringToULong(const char *str)
{
	unsigned long n = 0;
	while (isDigit(*str) == true && n <= (ULONG_MAX - (*str - '0')) / 10) {
		n = n * 10 + (*str - '0');
		str++;
	}
	if (*str != '\0') {
		return 0;
	}
	return n;
}
