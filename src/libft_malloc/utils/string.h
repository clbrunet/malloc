#ifndef LIBFT_MALLOC_UTILS_STRING_H
#define LIBFT_MALLOC_UTILS_STRING_H

#include <stdbool.h>
#include <stddef.h>

size_t getStringLength(const char *str);
void *memoryCopy(void *dst, const void *src, size_t n);
void *memorySet(void *dst, unsigned char c, size_t n);

unsigned char stringToUChar(const char *str);
unsigned long stringToULong(const char *str);

#endif
