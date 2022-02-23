#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "libft_malloc.h"

#define THREAD_NUMBER 1

void *routine()
{
	char *p = malloc(20);
	char *p2 = malloc(4);
	p = realloc(p, 300);
	for (size_t i = 0; i < 35; i++) {
		p[i] = i;
	}
	show_alloc_mem_ex();
	write(1, "\n\n", 2);
	free(p);
	free(p2);
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
