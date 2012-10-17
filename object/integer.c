#include "object/integer.h"
#include "object/vm.h"

#include <assert.h>
#include <string.h>

static void integer_fn_initialize(object_t *obj);
static void integer_fn_deinitialize(object_t *obj);
static wchar_t *integer_fn_repr(object_t *obj);
static size_t integer_fn_hash(object_t *obj);

integer_t *integer_new(int i)
{
	type_t *it = vm_get()->string_type;
	integer_t *obj = (integer_t*)type_allocate(it);
	type_initialize(it, (object_t*)obj);

	obj->i = i;

	return obj;
}

type_t *integertype_new()
{
	type_t *it = type_new(L"integer", vm_get()->object_type);
	it->basicsize = sizeof(struct integer);
	it->fn_initialize   = &integer_fn_initialize;
	it->fn_deinitialize = &integer_fn_deinitialize;
	it->fn_repr         = &integer_fn_repr;
	it->fn_hash         = &integer_fn_hash;
	return it;
}

/********************************************************************************/

static void integer_fn_initialize(object_t *obj)
{
	((integer_t*)obj)->i = 0;
}

static void integer_fn_deinitialize(object_t *obj)
{
}

static wchar_t *integer_fn_repr(object_t *obj)
{
	wchar_t tmp[16+1];
	swprintf(tmp, 16, L"%i", ((integer_t*)obj)->i);
	return wcsdup(tmp);
}

static size_t integer_fn_hash(object_t *obj)
{
	return ((integer_t*)obj)->i;
}
