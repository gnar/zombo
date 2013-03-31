#include "nil.h"

#include <string.h>

Object *nil_construct()
{
	return (Object*)0;
}

// Virtual methods ////////////////////////////////////////////

Object_vtab nil_vtab = {
	&object_vtab, /*super*/
	object_virt_message,
	nil_virt_get_size,
	nil_virt_internal_repr
};

size_t nil_virt_get_size(Object *self)
{
	return 0;
}

char  *nil_virt_internal_repr(Object *self)
{
	return strdup("nil");
}
