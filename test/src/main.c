#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "libft_malloc.h"

#define THREAD_NUMBER 3

void *routine()
{
	char *p = malloc(20);
	char *p2 = malloc(4);
	// p = realloc(p, 300);
	for (size_t i = 0; i < 20; i++) {
		p[i] = i;
	}
	show_alloc_mem();
	show_alloc_stats();
	free(p);
	free(p2);
	return NULL;
}

int main(int argc, char **argv)
{
	write(STDIN_FILENO, "go\n", 3);
	if (argc == 1) {
		pthread_t thread[THREAD_NUMBER];
		for (size_t i = 0; i < THREAD_NUMBER; i++) {
    		pthread_create(&thread[i], NULL, &routine, NULL);
		}
		for (size_t i = 0; i < THREAD_NUMBER; i++) {
    		pthread_join(thread[i], NULL);
		}
		show_alloc_mem_ex();
		return 0;
	}
	if (argv[1][0] == '0') {
	} else if (argv[1][0] == '1') {
		for (size_t i = 0; i < 1024; i++) {
			malloc(1024);
		}
		show_alloc_stats();
	}
	return 0;
}
