#include "nil.h"

#include <assert.h>
#include <string.h>

Nil *nil_construct(struct Class *clas, size_t bytes)
{
	Nil *n;

	assert(bytes >= sizeof(Nil));

	n = (Nil*)instance_construct(clas, bytes);
	n->super.super.vtab = &nil_vtab;
	return n;
}

/* Virtual methods */

Object_vtab nil_vtab = {
	&object_vtab, /*super*/
	object_virt_message,
	nil_virt_get_size,
	nil_virt_internal_repr
};

size_t nil_virt_get_size(Object *self)
{
	return sizeof(Nil);
}

char  *nil_virt_internal_repr(Object *self)
{
	return strdup("nil");
}
