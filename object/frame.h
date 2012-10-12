#ifndef FRAME_H
#define FRAME_H

#include "object/object.h"

typedef struct environment
{
	object_t super;

	struct environment *up;
} environment_t;

typedef struct closure
{
	object_t super;

	/* the compiled function body */
	function_t *func;

	/* the definition environment */
	environment_t *lex_env;
} closure_t;

typedef struct continuation
{
	object_t super;
	
	/* the target frame */
	struct frame *frame;
} continuation_t;

typedef struct frame
{
	object_t super;

	/* associated closure and current instruction pointer */
	closure_t *closure;
	size_t ip; /* index to closure->code->instr[ip] */

	/* lexical environment: storage for the local variables including 'self' (index 0) and 'return' (index 1) */
	environment_t *lex_env;
	/* dynamic environment */
	environment_t *dyn_env;

	/* stack */
	object_t **stack;
	size_t sp; /* stack top index */
} frame_t;

typedef struct thread
{
	object_t super;

	frame_t *frame;
} thread_t;

#endif
