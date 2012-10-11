#ifndef VM_H
#define VM_H

#include "object/object.h"

typedef struct vm
{
	struct object super;

	struct type *type_type;
	struct type *object_type;
	struct type *instance_type;
	struct type *string_type;
	struct type *nil_type;
	struct type *bool_type;
	struct type *list_type;
	struct type *symbol_type;
	struct type *vm_type;
	struct type *map_type;

	struct type *function_type;
	struct type *closure_type;
	struct type *frame_type;
	struct type *continuation_type;
	struct type *thread_type;

	struct nil *nil_obj;
	struct bool_ *true_obj, *false_obj;
} vm_t;

vm_t *vm_create();
vm_t *vm_get();
void vm_shutdown(vm_t *vm);

#endif
