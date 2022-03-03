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
	free(p);
	free(p2);
	write(1, "freed\n", 6);
	return NULL;
}

int main(int argc, char **argv)
{
	if (argc == 1) {
		void *p1 = malloc(20);
		show_alloc_mem();
		void *p2 = malloc(20);
		show_alloc_mem();
		void *p3 = malloc(20);
		show_alloc_mem();
		void *p4 = malloc(20);
		show_alloc_mem();
		free(p1);
		show_alloc_mem();
		free(p2);
		show_alloc_mem();
		free(p3);
		show_alloc_mem();
		free(p4);
		show_alloc_mem();
		write(1, "\n\n", 2);
		void *p5 = malloc(20);
		show_alloc_mem();
		free(p5);
		return 0;
		pthread_t thread[THREAD_NUMBER];
		for (size_t i = 0; i < THREAD_NUMBER; i++) {
    		pthread_create(&thread[i], NULL, &routine, NULL);
		}
		for (size_t i = 0; i < THREAD_NUMBER; i++) {
    		pthread_join(thread[i], NULL);
		}
		return 0;
	}
	if (argv[1][0] == '0') {
	} else if (argv[1][0] == '1') {
		for (size_t i = 0; i < 1024; i++) {
			malloc(1024);
		}
	}
	return 0;
}
