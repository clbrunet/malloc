#include <assert.h>
#include <unistd.h>
#include <limits.h>

#include "libft_malloc/utils/string.h"

void printChar(char c)
{
	write(1, &c, 1);
}

void printEndl()
{
	printChar('\n');
}

void printStr(const char *str)
{
	assert(str != NULL);

	write(1, str, getStringLength(str));
}

void printNbr(long long nbr)
{
	unsigned long long u_nbr;
	if (nbr == LLONG_MIN) {
		write(1, "-", 1);
		u_nbr = (unsigned long long)LLONG_MAX + 1;
	} else if (nbr < 0) {
		write(1, "-", 1);
		u_nbr = -nbr;
	} else {
		u_nbr = nbr;
	}

	if (u_nbr >= 10) {
		printNbr(u_nbr / 10);
	}
	printChar(u_nbr % 10 + '0');
}

void printHex(long long hex)
{
	unsigned long long u_hex;
	if (hex == LLONG_MIN) {
		write(1, "-", 1);
		u_hex = (unsigned long long)LLONG_MAX + 1;
	} else if (hex < 0) {
		write(1, "-", 1);
		u_hex = -hex;
	} else {
		u_hex = hex;
	}

	const char *chars = "0123456789ABCDEF";

	if (u_hex >= 16) {
		printHex(u_hex / 16);
	}
	printChar(chars[u_hex % 16]);
}

void printHexMinimumLength(long long hex, size_t minimum_length)
{
	if (minimum_length <= 1) {
		return printHex(hex);
	}

	unsigned long long u_hex;
	if (hex == LLONG_MIN) {
		u_hex = (unsigned long long)LLONG_MAX + 1;
	} else if (hex < 0) {
		u_hex = -hex;
	} else {
		u_hex = hex;
	}

	while (minimum_length > 0 && u_hex >= 16) {
		u_hex /= 16;
		minimum_length--;
	}
	minimum_length--;
	while (minimum_length > 0) {
		printChar('0');
		minimum_length--;
	}
	printHex(hex);
}
