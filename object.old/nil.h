#ifndef NIL_H
#define NIL_H

#include "instance.h"

extern Object_vtab nil_vtab;
size_t nil_virt_get_size(Object *self);
char  *nil_virt_internal_repr(Object *self);

typedef struct Nil
{
	Instance super;
} Nil;

Nil *nil_construct(struct Class *clas, size_t bytes);

#endif
