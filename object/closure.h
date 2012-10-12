#ifndef CLOSURE_H
#define CLOSURE_H

#include "object/object.h"
#include "object/function.h"

typedef struct environment 
{
	object_t super;
} environment_t;

typedef struct closure
{
	object_t super;

	function_t *func; /* the compiled function body */
	environment_t *lex_env; /* the definition environment */
	environment_t *dyn_env; /* the caller environment */
} closure_t;

#endif
