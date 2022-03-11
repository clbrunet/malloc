#ifndef LIBFT_MALLOC_DEBUG_VARIABLES_H
#define LIBFT_MALLOC_DEBUG_VARIABLES_H

#include <stdbool.h>

typedef struct debug_variables_s
{
	bool is_initialized;
	unsigned char perturb_byte;
	unsigned int fail_at;
} debug_variables_t;

void setDebugVariables(debug_variables_t *debug_variables);

#endif
