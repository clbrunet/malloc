#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft_malloc.h"

int main()
{
	char *p = malloc(6);
	show_alloc_mem();
	p = realloc(p, 10500);
	show_alloc_mem();
	return 0;
}
