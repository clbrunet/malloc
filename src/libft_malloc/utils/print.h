#ifndef LIBFT_MALLOC_UTILS_PRINT_H
#define LIBFT_MALLOC_UTILS_PRINT_H

#include <stddef.h>

void printChar(char c);
void printEndl();
void printStr(const char *str);
void printNbr(long long nbr);
void printHex(long long hex);
void printHexMinimumLength(long long hex, size_t minimum_length);

#endif
