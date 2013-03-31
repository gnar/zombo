#ifndef INSTANCE_H
#define INSTANCE_H

#include "object.h"

extern Object_vtab instance_vtab;
Object *instance_virt_message(Object *self, char *id);
size_t instance_virt_get_size(Object *self);

typedef struct Instance
{
	Object super;
} Instance;

Instance *instance_construct(struct Class *clas, size_t bytes);

#endif
