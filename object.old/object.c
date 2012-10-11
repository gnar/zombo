#include "object.h"
#include "class.h"
#include "nil.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Object *object_construct(struct Class *clas, size_t bytes)
{
	Object *self;

	assert(bytes >= sizeof(Object));

	self = (Object*)malloc(bytes);
	self->clas = clas;
	self->vtab = &object_vtab;

	return self;
}

Object_vtab *object_get_vtab(struct Object *self)
{
	return self->vtab;
}

struct Class *object_get_class(Object *self)
{
	return self->clas;
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

/* Virtual methods */

struct Object_vtab object_vtab = {
	NULL, /*super*/
	object_virt_message, /* object_message() */
	NULL, /* get_size() [abstract] */
	object_virt_internal_repr
};

Object *object_virt_message(Object *self, char *id)
{
	/* Look in class methods */
	return class_get_method(object_get_class(self), id);
}

char *object_virt_internal_repr(Object *self)
{
	char buf[32];
	snprintf(buf, 32, "<object@%p>", (void*)self);
	return strdup(buf);
}
