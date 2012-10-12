#include "object/bool.h"
#include "object/string.h"
#include "object/vm.h"
#include "object/type.h"

#include "tools.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void bool_fn_initialize(object_t *obj)
{
	bool_t *b = (bool_t*)obj;
	b->value = false;
}

static void bool_fn_deinitialize(object_t *obj)
{
}

static wchar_t *bool_fn_repr(object_t *obj)
{
	bool_t *b = (bool_t*)obj;
	if (bool_is_true(b)) {
		return wcsdup(L"true");
	} else {
		return wcsdup(L"false");
	}
}

static size_t bool_fn_hash(object_t *self)
{
	return bool_is_true((bool_t*)self) ? 1 : 0;
}

type_t *booltype_new()
{
	type_t *nt = (type_t*)type_new(L"bool", vm_get()->object_type);
	nt->basicsize = sizeof(struct bool_);
	nt->fn_initialize = &bool_fn_initialize;
	nt->fn_deinitialize = &bool_fn_deinitialize;
	nt->fn_repr = &bool_fn_repr;
	nt->fn_hash = &bool_fn_hash;
	return nt;
}

bool_t *bool_new(bool value)
{
	type_t *bool_type = vm_get()->bool_type;
	bool_t *self = (bool_t*)type_allocate(bool_type);
	type_initialize(bool_type, (object_t*)self);

	self->value = value;
	return self;
}

bool bool_check(object_t *self)
{
	return is_instance(self, vm_get()->bool_type);
}

bool bool_is_true(bool_t *self)
{
	return self->value;
}
