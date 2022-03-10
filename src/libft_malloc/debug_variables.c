#include <limits.h>
#include <stdlib.h>
#include <assert.h>

#include "libft_malloc/debug_variables.h"
#include "libft_malloc/utils/char_type.h"
#include "libft_malloc/utils/print.h"

static unsigned char parsePerturbValue(const char *value)
{
	unsigned char perturb_byte = 0;
	while (isDigit(*value) == true && perturb_byte <= (UCHAR_MAX - (*value - '0')) / 10) {
		perturb_byte = perturb_byte * 10 + (*value - '0');
		value++;
	}
	if (*value != '\0') {
		return 0;
	}
	return perturb_byte;
}

void setDebugVariables(debug_variables_t *debug_variables)
{
	assert(debug_variables != NULL);

	char *perturb_env_value = getenv("MALLOC_PERTURB_");
	if (perturb_env_value != NULL) {
		debug_variables->perturb_byte = parsePerturbValue(perturb_env_value);
	} else {
		debug_variables->perturb_byte = 0;
	}

	debug_variables->is_initialized = true;
}
