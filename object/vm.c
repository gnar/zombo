#include "object/vm.h"
#include "object/object.h"
#include "object/type.h"
#include "object/nil.h"
#include "object/bool.h"
#include "object/string.h"
#include "object/symbol.h"
#include "object/map.h"
#include "object/function.h"

#include <stdlib.h>
#include <string.h>

static struct vm *the_itp = NULL;

wchar_t *vm_fn_repr(struct object *self)
{
	return object_fn_repr(self);
}

static type_t *vmtype_new()
{
	type_t *nt = (type_t*)type_new(L"vm", vm_get()->object_type);
	nt->basicsize = sizeof(struct vm);
	nt->fn_allocate = &object_fn_allocate;
	nt->fn_deallocate = &object_fn_deallocate;
	nt->fn_repr = &vm_fn_repr;
	return nt;
}

vm_t *vm_get()
{
	return the_itp;
}

vm_t *vm_create()
{
	struct vm *itp = the_itp = (vm_t*)malloc(sizeof(struct vm));
	itp->super.type = NULL; /* see (1) */
	itp->super.ref_cnt = 1;

	/*** I. Setup garbage collector ***/
	//TODO

	/*** II. Construct the types ***/
	
	/* Create "object" and "type" types */
	itp->object_type = (type_t*)malloc(sizeof(struct type));
	itp->type_type = (type_t*)malloc(sizeof(struct type));
	itp->instance_type = NULL; // TODO

	typetype_bootstrap(itp);
	objecttype_bootstrap(itp);

	/* "nil" type */
	itp->nil_type = niltype_new();
	nil_bootstrap(itp); /* create the nil object */

	/* "bool" type */
	itp->bool_type = booltype_new();
	bool_bootstrap(itp); /* create the true and false objects */

	/* string type */
	itp->string_type = stringtype_new();

	/* symbol type */
	itp->symbol_type = symboltype_new();

	/* map type */
	itp->map_type = maptype_new();
	
	/* list type */
	itp->list_type = NULL;

	/* vm type */
	itp->vm_type = vmtype_new();
	itp->super.type = itp->vm_type; /* (1) */

	/* function_type */
	itp->function_type = functiontype_new();
	itp->closure_type = NULL;
	itp->frame_type = NULL;
	itp->continuation_type = NULL;
	itp->thread_type = NULL;

	return itp;
}

void vm_shutdown(vm_t *vm)
{
	//DECR0(vm->type_type);
	//DECR0(vm->object_type);
	//DECR0(vm->instance_type);
	//DECR0(vm->string_type);
	//DECR0(vm->nil_type);
	//DECR0(vm->bool_type);
	//DECR0(vm->list_type);
	//DECR0(vm->symbol_type);
	//DECR0(vm->vm_type);
	//DECR0(vm->map_type);

	DECR0(vm->function_type);
	DECR0(vm->closure_type);
	DECR0(vm->frame_type);
	DECR0(vm->continuation_type);
	DECR0(vm->thread_type);

	DECR0(vm->nil_obj);
	DECR0(vm->true_obj);
	DECR0(vm->false_obj);
}
