#include "object/vm.h"
#include "object/object.h"
#include "object/type.h"
#include "object/nil.h"
#include "object/bool.h"
#include "object/string.h"
#include "object/symbol.h"
#include "object/map.h"
#include "object/function.h"
#include "object/thread.h"

#include <stdlib.h>
#include <string.h>

static struct vm *the_itp = NULL;

wchar_t *vm_fn_repr(struct object *self)
{
	return object_fn_repr(self);
}

vm_t *vm_get()
{
	return the_itp;
}

vm_t *vm_create()
{
	wchar_t *type_fn_repr(object_t*);
	void type_fn_initialize(object_t*);
	void type_fn_deinitialize(object_t*);

	/*** I. Setup garbage collector ***/
	//TODO

	/*** II. Construct the most basic types and objects ***/

	/** Bootstrap the "object" and "type" and "vm" types **/
	
	type_t *tt = (type_t*)malloc(sizeof(type_t)); tt->super.ref_cnt = 1; tt->super.type = tt; INCR(tt);
	type_t *ot = (type_t*)malloc(sizeof(type_t)); ot->super.ref_cnt = 1; ot->super.type = tt; INCR(tt);
	type_t *vt = (type_t*)malloc(sizeof(type_t)); vt->super.ref_cnt = 1; vt->super.type = tt; INCR(tt);

	tt->name = wcsdup(L"type");
	tt->base = ot; INCR(tt->base);
	tt->basicsize = sizeof(struct type);
	tt->fn_allocate     = &object_fn_allocate;
	tt->fn_deallocate   = &object_fn_deallocate;
	tt->fn_initialize   = &type_fn_initialize;
	tt->fn_deinitialize = &type_fn_deinitialize;
	tt->fn_repr         = &type_fn_repr;
	tt->fn_hash         = &object_fn_hash;

	ot->name = wcsdup(L"object");
	ot->base = NULL; INCR(ot->base);
	ot->basicsize = sizeof(struct type);
	ot->fn_allocate     = &object_fn_allocate;
	ot->fn_deallocate   = &object_fn_deallocate;
	ot->fn_initialize   = &object_fn_initialize;
	ot->fn_deinitialize = &object_fn_deinitialize;
	ot->fn_repr         = &object_fn_repr;
	ot->fn_hash         = &object_fn_hash;

	vt->name = wcsdup(L"vm");
	vt->base = ot; INCR(vt->base);
	vt->basicsize = sizeof(struct type);
	vt->fn_allocate     = &object_fn_allocate;
	vt->fn_deallocate   = &object_fn_deallocate;
	vt->fn_initialize   = &object_fn_initialize;
	vt->fn_deinitialize = &object_fn_deinitialize;
	vt->fn_repr         = &vm_fn_repr;
	vt->fn_hash         = &object_fn_hash;

	/** Bootstrap the vm object **/
	vm_t *itp = the_itp = (vm_t*)malloc(sizeof(vm_t)); itp->super.ref_cnt = 1; itp->super.type = vt; INCR(itp->super.type);
	itp->type_type = tt;
	itp->object_type = ot;
	itp->vm_type = vt;

	/*** III. Construct the basic types and objects (all the weird bootstrapping is done by now) ***/

	/* "nil" type */
	itp->nil_type = niltype_new();
	itp->nil_obj = nil_new(); /* also create the default nil object */

	/* "bool" type */
	itp->bool_type = booltype_new();
	itp->true_obj = bool_new(true);
	itp->false_obj = bool_new(false);

#if 0
	/* string type */
	itp->string_type = stringtype_new();

	/* symbol type */
	itp->symbol_type = symboltype_new();

	/* map type */
	itp->map_type = maptype_new();
	
	/* list type */
	itp->list_type = NULL;

	/* instance_type */
	itp->instance_type = NULL;

	/* function_type */
	itp->function_type = functiontype_new();
	itp->closure_type = NULL;
	itp->frame_type = NULL;
	itp->continuation_type = NULL;
	itp->thread_type = threadtype_new();
#endif

	return itp;
}

void vm_shutdown(vm_t *vm)
{
	DECR(vm->type_type);
	DECR(vm->object_type);
	DECR(vm->vm_type);

	DECR(vm->nil_type);
	DECR(vm->bool_type);
	/*DECR(vm->string_type);
	DECR(vm->list_type);
	DECR(vm->symbol_type);
	DECR(vm->map_type);
	DECR(vm->instance_type);

	DECR(vm->function_type);
	DECR(vm->closure_type);
	DECR(vm->frame_type);
	DECR(vm->continuation_type);
	DECR(vm->thread_type);*/

	DECR(vm->nil_obj);
	DECR(vm->true_obj);
	DECR(vm->false_obj);

	// - run gc
	// - delete remaining objects
}
