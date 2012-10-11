#include "object/bool.h"
#include "object/string.h"
#include "object/vm.h"
#include "object/type.h"

#include "tools.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

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

static object_t *bool_fn_allocate(type_t *type)
{
	return (object_t*)vm_get()->false_obj;
}

static void bool_fn_deallocate(object_t *self)
{
	/* no op */
}

type_t *booltype_new()
{
	type_t *nt = (type_t*)type_new(L"bool", vm_get()->object_type);
	nt->basicsize = sizeof(struct bool_);
	nt->fn_allocate = &bool_fn_allocate;
	nt->fn_deallocate = &bool_fn_deallocate;
	nt->fn_repr = &bool_fn_repr;
	nt->fn_hash = &bool_fn_hash;
	return nt;
}

bool_t *bool_new(bool value)
{
	if (value == true) {
		return vm_get()->true_obj;
	} else {
		return vm_get()->false_obj;
	}
}

bool bool_check(object_t *self)
{
	return is_instance(self, vm_get()->bool_type);
}

bool bool_is_true(bool_t *self)
{
	vm_t *itp = vm_get();
	if (self == itp->true_obj ) return true;
	if (self == itp->false_obj) return false;
	assert(0);
	return false;
}

void bool_bootstrap(struct vm *itp)
{
	itp->true_obj  = (bool_t*)malloc(sizeof(struct bool_));
	itp->true_obj->super.type = itp->bool_type;
	itp->true_obj->super.ref_cnt = 1;

	itp->false_obj = (bool_t*)malloc(sizeof(struct bool_));
	itp->false_obj->super.type = itp->bool_type;
	itp->false_obj->super.ref_cnt = 1;
}
