#include "object/nil.h"
#include "object/string.h"
#include "object/vm.h"
#include "object/type.h"

#include "tools.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static wchar_t *nil_fn_repr(object_t *self)
{
	return wcsdup(L"nil");
}

static size_t nil_fn_hash(object_t *self)
{
	return 42; /* TODO */
}

type_t *niltype_new()
{
	type_t *nt = (type_t*)type_new(L"nil_t", vm_get()->object_type);
	nt->basicsize = sizeof(struct nil);
	nt->fn_allocate = &object_fn_allocate;
	nt->fn_deallocate = &object_fn_deallocate;
	nt->fn_repr = &nil_fn_repr;
	nt->fn_hash = &nil_fn_hash;
	return nt;
}

nil_t *nil_new()
{
	return (nil_t*)type_allocate(vm_get()->nil_type);
	/* skipped call to type_initialize */
}

bool nil_check(object_t *self)
{
	return is_instance(self, vm_get()->nil_type);
}
