#include "object/frame.h"

#include <string.h>

frame_t *frame_duplicate(struct frame *frm)
{
	frame_t *dup = (frame_t*)type_allocate(vm_get()->frame_type, frm); type_initialize(vm_get()->frame_type, (object_t*)frm);

	dup->clos    = frm->clos; INCR(dup->clos);
	dup->ip      = frm->ip;

	dup->dyn_env = frm->dyn_env; INCR(dup->dyn_env);

	const size_t stack_size = frm->clos->func->stack_size;
	dup->stack   = (object_t**)malloc(stack_size * sizeof(object_t*));
	dup->sp      = frm->sp;
	for (size_t i=0; i<frm->sp; ++i) {
		object_t *item = frm->stack[i];
		dup->stack[i] = item; INCR(item);
	}
	return dup;
}
