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
	unsigned long long u_nbr;

	if (hex == LLONG_MIN) {
		write(1, "-", 1);
		u_nbr = (unsigned long long)LLONG_MAX + 1;
	} else if (hex < 0) {
		write(1, "-", 1);
		u_nbr = -hex;
	} else {
		u_nbr = hex;
	}

	const char *chars = "0123456789ABCDEF";

	if (u_nbr >= 16) {
		printHex(u_nbr / 16);
	}
	printChar(chars[u_nbr % 16]);
}
