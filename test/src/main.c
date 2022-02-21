#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "libft_malloc.h"

void *routine()
{
	char *p = malloc(6);
	show_alloc_mem();
	free(p);
	return NULL;
}

int main()
{
	pthread_t thread[4];


	for (size_t i = 0; i < 4; i++) {
    	pthread_create(&thread[i], NULL, &routine, NULL);
	}
	for (size_t i = 0; i < 4; i++) {
    	pthread_join(thread[i], NULL);
	}
	return 0;
}
