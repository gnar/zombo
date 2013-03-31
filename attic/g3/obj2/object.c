#include "object.h"
#include "class.h"
#include "nil.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define IS_NIL(obj) (obj == (Object*)0)
#define IS_OBJ(obj) (obj != (Object*)0)

Object *object_construct(struct Class *clas, size_t bytes)
{
	assert(bytes >= sizeof(Object));

	Object *self = (Object*)malloc(bytes);
	self->clas = clas;
	self->vtab = &object_vtab;
}

Object_vtab *object_get_vtab(struct Object *self)
{
	if (IS_NIL(self)) {
		return &nil_vtab;
	} else {
		return self->vtab;
	}
}

struct Class *object_get_class(Object *self)
{
	if (IS_NIL(self)) {
		assert(0);
	} else {
		return self->clas;
	}
}

Object *object_message(Object *self, char *id)
{
	return object_get_vtab(self)->message(self, id);
}

size_t object_get_size(Object *self)
{
	return object_get_vtab(self)->get_size(self);
}

char *object_internal_repr(Object *self)
{
	return object_get_vtab(self)->internal_repr(self);
}

// Virtual methods ////////////////////////////////////////////

struct Object_vtab object_vtab = {
	0, /*super*/
	object_virt_message, /* object_message() */
	0, /* get_size() [abstract] */
	object_virt_internal_repr
};

Object *object_virt_message(Object *self, char *id)
{
	// Look in class methods
	return class_get_method(object_get_class(self), id);
}

char *object_virt_internal_repr(Object *self)
{
	char buf[32];
	snprintf(buf, 32, "<object@%p>", (void*)self);
	return strdup(buf);
}
