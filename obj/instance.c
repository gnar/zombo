#include "instance.h"

#include <assert.h>

Instance *instance_construct(struct Class *class, size_t bytes)
{
	Instance *c;

	assert(bytes >= sizeof(Instance));

	c = (Instance*)object_construct(class, bytes);
	c->super.vtab = &instance_vtab;
	return c;
}

/* Virtual methods */

Object_vtab instance_vtab = {
	&object_vtab, /*super*/
	instance_virt_message, /*override*/
	instance_virt_get_size, /*override*/
	object_virt_internal_repr /*override*/
};

Object *instance_virt_message(Object *self0, char *id)
{
	/* 1. Search in singleton methods */
	//Instance *self = (Instance*)self0;

	/* 2. Else, behaviour of Object */
	return object_get_vtab(self0)->super->message(self0, id);
}

size_t instance_virt_get_size(Object *self0)
{
	return sizeof(Instance);
}
