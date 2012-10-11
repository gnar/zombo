#include "tools.h"

#include "object/object.h"
#include "object/type.h"
#include "object/string.h"

#include <stdlib.h>
#include <assert.h>

bool is_instance(object_t *obj, type_t *type)
{
	type_t *t = obj->type; assert(t);
	while (t != type) {
		t = t->base;
		if (!t) return false; /* this happens after we reach the object type */
	}
	return true;
}

struct string *repr(object_t *self)
{
	wchar_t *r = self->type->fn_repr(self);
	string_t *rep = string_new(r);
	free(r);
	return rep;
}
