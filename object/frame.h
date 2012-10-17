#ifndef FRAME_H
#define FRAME_H

#include "object/object.h"

typedef struct frame
{
	object_t super;

	/* associated closure and current instruction pointer */
	closure_t *clos;
	size_t ip; /* index to closure->func->code->instr[ip] */

	/* dynamic environment */
	environment_t *dyn_env;

	/* stack */
	object_t **stack;
	size_t stack_size;
	size_t sp; /* stack top index */
} frame_t;

frame_t *frame_duplicate(struct frame *frm);

#endif
