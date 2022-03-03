#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>

#include "libft_malloc/utils/print.h"
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

void printHex(long long hex, bool should_prefix)
{
	unsigned long long u_hex;
	if (hex == LLONG_MIN) {
		printChar('-');
		u_hex = (unsigned long long)LLONG_MAX + 1;
	} else if (hex < 0) {
		printChar('-');
		u_hex = -hex;
	} else {
		u_hex = hex;
	}

	const char *chars = "0123456789ABCDEF";

	if (u_hex >= 16) {
		printHex(u_hex / 16, should_prefix);
	} else {
		if (should_prefix == true) {
			printStr("0x");
		}
	}
	printChar(chars[u_hex % 16]);
}

void printHexMinimumLength(long long hex, size_t minimum_length, bool should_prefix)
{
	size_t length = 1;
	unsigned long long u_hex;
	if (hex < 0) {
		printChar('-');
		length++;
		if (hex == LLONG_MIN) {
			u_hex = (unsigned long long)LLONG_MAX + 1;
		} else {
			u_hex = -hex;
		}
	} else {
		u_hex = hex;
	}

	if (length >= minimum_length) {
		return printHex(u_hex, should_prefix);
	}

	unsigned long long u_hex_backup = u_hex;
	while (u_hex >= 16) {
		u_hex /= 16;
		length++;
	}
	if (should_prefix == true) {
		printStr("0x");
	}
	while (length < minimum_length) {
		printChar('0');
		length++;
	}
	printHex(u_hex_backup, false);
}
