#ifndef LIBFT_MALLOC_UTILS_PRINT_H
#define LIBFT_MALLOC_UTILS_PRINT_H

#include <stdbool.h>
#include <stddef.h>

void printChar(char c);
void printEndl();
void printStr(const char *str);
void printNbr(long long nbr);
void printHex(long long hex, bool should_prefix);
void printHexMinimumLength(long long hex, size_t minimum_length, bool should_prefix);

#endif
