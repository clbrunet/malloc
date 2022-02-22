#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "libft_malloc.h"

#define THREAD_NUMBER 1

void *routine()
{
	char *p = malloc(2001);
	for (size_t i = 0; i < 35; i++) {
		p[i] = i;
	}
	show_alloc_mem_ex();
	write(1, "\n\n", 2);
	free(p);
	return NULL;
}

int main()
{
	pthread_t thread[THREAD_NUMBER];


	for (size_t i = 0; i < THREAD_NUMBER; i++) {
    	pthread_create(&thread[i], NULL, &routine, NULL);
	}
	for (size_t i = 0; i < THREAD_NUMBER; i++) {
    	pthread_join(thread[i], NULL);
	}
	return 0;
}
