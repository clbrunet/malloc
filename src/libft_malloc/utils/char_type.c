#include "libft_malloc/utils/char_type.h"

bool isDigit(int c)
{
	if ('0' <= c && c <= '9') {
		return true;
	}
	return false;
}
