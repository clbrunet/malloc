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
	const char *possible_arguments = "possible arguments : "
		"nothing, malloc, free, threads, perturb, fail_at, max_bytes, history\n";
	write(STDIN_FILENO, possible_arguments, strlen(possible_arguments));
}

static void mallocTests()
{
	for (size_t i = 0; i < 1024; i++) {
		malloc(1024);
	}
	show_alloc_stats();
}

static void freeTests()
{
	char *ptrs[1024];
	for (size_t i = 0; i < 1024; i++) {
		ptrs[i] = malloc(1024);
	}
	for (size_t i = 0; i < 1024; i++) {
		free(ptrs[i]);
	}
	show_alloc_stats();
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

static void perturbTests()
{
	const char *msg = "MALLOC_PERTURB_ should be set to a value between 1 and 254\n";
	write(STDIN_FILENO, msg, strlen(msg));
	void *to_not_delete_zone_after_p_free = malloc(1);
	unsigned char *p = malloc(10);
	for (size_t i = 0; i < 10; i++) {
		printf("i: %zu, p[i]: %d\n", i, p[i]);
	}
	free(p);
	printf("\n");
	for (size_t i = 0; i < 10; i++) {
		printf("i: %zu, p[i]: %d\n", i, p[i]);
	}
	free(to_not_delete_zone_after_p_free);
}

static void failAtTests()
{
	const char *msg = "MALLOC_FAIL_AT_ should be set to 2\n";
	write(STDIN_FILENO, msg, strlen(msg));
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

static void historyTests()
{
	const char *msg = "MALLOC_ENABLE_HISTORY_ should be set to a nonzero value\n";
	write(STDIN_FILENO, msg, strlen(msg));
	void *p1 = malloc(10);
	void *p2 = malloc(10);
	p1 = realloc(p1, 15);
	void *p3 = malloc(10);
	show_alloc_mem_ex();
	free(p1);
	free(p2);
	free(p3);
	show_alloc_mem_ex();
}

static void maxBytesTests()
{
	const char *msg = "MALLOC_MAX_BYTES_ should be set to 15\n";
	write(STDIN_FILENO, msg, strlen(msg));
	void *p1 = malloc(10);
	void *p2 = malloc(10);
	printf("p1: %p\n", p1);
	printf("p2: %p\n", p2);
	free(p1);
	p2 = malloc(10);
	printf("after free(p1) : p2: %p\n", p2);
	free(p2);
}

int main(int argc, char **argv)
{
	write(STDIN_FILENO, "GO\n", 3);
	if (argc == 1) {
		printUsage(argv[0]);
		return 0;
	}

	if (strcmp(argv[1], "nothing") == 0) {
	} else if (strcmp(argv[1], "malloc") == 0) {
		mallocTests();
	} else if (strcmp(argv[1], "free") == 0) {
		freeTests();
	} else if (strcmp(argv[1], "threads") == 0) {
		threadsTests();
	} else if (strcmp(argv[1], "perturb") == 0) {
		perturbTests();
	} else if (strcmp(argv[1], "fail_at") == 0) {
		failAtTests();
	} else if (strcmp(argv[1], "max_bytes") == 0) {
		maxBytesTests();
	} else if (strcmp(argv[1], "history") == 0) {
		historyTests();
	} else {
		const char *wrong_argument = "wrong argument\n";
		write(STDIN_FILENO, wrong_argument, strlen(wrong_argument));
		printUsage(argv[0]);
	}
	return 0;
}
