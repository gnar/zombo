#include "object/object.h"
#include "object/vm.h"
#include "object/type.h"
#include "object/string.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

object_t *object_fn_allocate(struct type *type)
{
	object_t *obj = (object_t*)malloc(type->basicsize);
	obj->type = type;
	obj->ref_cnt = 1;
	return obj;
}

void object_fn_deallocate(struct object *self)
{
	free(self);
}

void object_fn_initialize(struct object *self)
{
	/* no op */
}

void object_fn_deinitialize(struct object *self)
{
	/* no op */
}

wchar_t *object_fn_repr(struct object *self)
{
	wchar_t tmp[64];
	swprintf(tmp, 63, L"<%ls@%p>", self->type->name, (void*)self);
	return wcsdup(tmp);
}

size_t object_fn_hash(struct object *self)
{
	return 0;
}

void object_delete(struct object *obj)
{
	printf("object_delete: %p\n", obj);
	type_deinitialize(obj->type, obj);
	type_deallocate(obj->type, obj);
}

void objecttype_bootstrap(struct vm *itr)
{
	type_t *ot = itr->object_type;
	
	ot->super.type = itr->type_type;
	ot->super.ref_cnt = 1;

	ot->base = NULL;
	ot->basicsize = sizeof(struct type);

	ot->fn_allocate     = &object_fn_allocate;
	ot->fn_deallocate   = &object_fn_deallocate;
	ot->fn_initialize   = &object_fn_initialize;
	ot->fn_deinitialize = &object_fn_deinitialize;
	ot->fn_repr         = &object_fn_repr;
	ot->fn_hash         = &object_fn_hash;
	ot->fn_hash         = &object_fn_hash;
}
