#include "object/type.h"
#include "object/object.h"
#include "object/vm.h"
#include "object/string.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static wchar_t *type_fn_repr(object_t *obj)
{
	type_t *type = (type_t*)obj;
	return wcsdup(type->name);
}

static void type_fn_initialize(object_t *o_self)
{
	type_t *self = (type_t*)o_self;
	type_t *base = vm_get()->object_type;

	self->name = wcsdup(L"<unnamed type>");
	self->base = base; INCR(self->base);
	self->basicsize = sizeof(struct object);

	self->fn_allocate     = base->fn_allocate;
	self->fn_deallocate   = base->fn_deallocate;
	self->fn_initialize   = NULL;
	self->fn_deinitialize = NULL;
	self->fn_repr         = &type_fn_repr;
}

static void type_fn_deinitialize(object_t *o_self)
{
	type_t *self = (type_t*)o_self;
	free(self->name); self->name = NULL;

	DECR0(self->base);
	self->basicsize = 0;

	self->fn_allocate     = NULL;
	self->fn_deallocate   = NULL;
	self->fn_initialize   = NULL;
	self->fn_deinitialize = NULL;
	self->fn_repr         = NULL;
}

void typetype_bootstrap(struct vm *itr)
{
	type_t *tt = itr->type_type;
	
	tt->super.type = itr->type_type;    /* type of type-object is type */
	tt->super.ref_cnt = 1;

	tt->name = wcsdup(L"type");
	tt->base = itr->object_type; /* type is derived from object */
	tt->basicsize = sizeof(struct type);

	tt->fn_allocate     = &object_fn_allocate;
	tt->fn_deallocate   = &object_fn_deallocate;
	tt->fn_initialize   = &type_fn_initialize;
	tt->fn_deinitialize = &type_fn_deinitialize;
	tt->fn_repr = &type_fn_repr;
	tt->fn_hash = &object_fn_hash;
}

type_t *type_new(const wchar_t *name, type_t *base)
{
	type_t *self = (type_t*)type_allocate(vm_get()->type_type);
	type_initialize(vm_get()->type_type, (object_t*)self);

	self->name = wcsdup(name);
	self->base = base;
	self->basicsize = base->basicsize;

	self->fn_allocate     = base->fn_allocate;
	self->fn_deallocate   = base->fn_deallocate;
	self->fn_initialize   = base->fn_initialize;
	self->fn_deinitialize = base->fn_deinitialize;
	self->fn_repr         = base->fn_repr;
	self->fn_hash         = base->fn_hash;

	return self;
}

object_t *type_allocate(type_t *type)
{
	return type->fn_allocate(type);
}

void type_deallocate(type_t *type, object_t *obj)
{
	type->fn_deallocate(obj);
}

void type_initialize(type_t *type, object_t *obj)
{
	if (type->fn_initialize) type->fn_initialize(obj);
}

void type_deinitialize(type_t *type, object_t *obj)
{
	if (type->fn_deinitialize) type->fn_deinitialize(obj);
}
