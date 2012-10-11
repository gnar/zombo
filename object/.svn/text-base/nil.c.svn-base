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

static object_t *nil_fn_allocate(type_t *type)
{
	return (object_t*)vm_get()->nil_obj;
}

static void nil_fn_deallocate(object_t *self)
{
	/* no op */
}

type_t *niltype_new()
{
	type_t *nt = (type_t*)type_new(L"nil", vm_get()->object_type);
	nt->basicsize = sizeof(struct nil);
	nt->fn_allocate = &nil_fn_allocate;
	nt->fn_deallocate = &nil_fn_deallocate;
	nt->fn_repr = &nil_fn_repr;
	nt->fn_hash = &nil_fn_hash;
	return nt;
}

nil_t *nil_new()
{
	return vm_get()->nil_obj;
	/* same behaviour as, but faster than:
	return object_new(vm_get()->nil_type);
	*/
}

bool nil_check(object_t *self)
{
	return is_instance(self, vm_get()->nil_type);
}

void nil_bootstrap(struct vm *itp)
{
	itp->nil_obj = (nil_t*)malloc(sizeof(struct nil));
	itp->nil_obj->super.type = itp->nil_type;
	itp->nil_obj->super.ref_cnt = 1;
}
