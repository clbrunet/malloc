#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "libft_malloc.h"

#define THREAD_NUMBER 3

static void printUsage(const char *argv0)
{
	const char *usage = "usage: ";
	write(STDIN_FILENO, usage, strlen(usage));
	write(STDIN_FILENO, argv0, strlen(argv0));
	const char *argument = " argument\n";
	write(STDIN_FILENO, argument, strlen(argument));
	const char *possible_arguments = "possible arguments : 0, 1, threads, fail_at\n";
	write(STDIN_FILENO, possible_arguments, strlen(possible_arguments));
}

static void *routine()
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

static void threadsTests()
{
	pthread_t thread[THREAD_NUMBER];
	for (size_t i = 0; i < THREAD_NUMBER; i++) {
		pthread_create(&thread[i], NULL, &routine, NULL);
	}
	for (size_t i = 0; i < THREAD_NUMBER; i++) {
		pthread_join(thread[i], NULL);
	}
	show_alloc_mem_ex();
}

static void fail_atTests()
{
	void *p1 = malloc(10);
	void *p2 = malloc(10);
	void *p3 = malloc(10);
	printf("p1 %p\n", p1);
	printf("p2 %p\n", p2);
	printf("p3 %p\n", p3);
	free(p1);
	free(p2);
	free(p3);
}

int main(int argc, char **argv)
{
	write(STDIN_FILENO, "go\n", 3);
	if (argc == 1) {
		printUsage(argv[0]);
		return 0;
	}

	if (strcmp(argv[1], "0") == 0) {
		show_alloc_stats();
	} else if (strcmp(argv[1], "1") == 0) {
		for (size_t i = 0; i < 1024; i++) {
			malloc(1024);
		}
		show_alloc_stats();
	} else if (strcmp(argv[1], "threads") == 0) {
		threadsTests();
	} else if (strcmp(argv[1], "fail_at") == 0) {
		fail_atTests();
	} else {
		const char *wrong_argument = "wrong argument\n";
		write(STDIN_FILENO, wrong_argument, strlen(wrong_argument));
		printUsage(argv[0]);
	}
	return 0;
}
