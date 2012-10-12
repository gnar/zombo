#ifndef CLOSURE_H
#define CLOSURE_H

#include "object/object.h"

typedef struct closure
{
	object_t super;

	function_t *func; /* the compiled function body */
	environment_t *lex_env; /* the definition environment */
	environment_t *dyn_env; /* the caller environment */
} closure_t;

#endif
