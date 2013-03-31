#ifndef NIL_H
#define NIL_H

#include "object.h"

extern Object_vtab nil_vtab;
size_t nil_virt_get_size(Object *self);
char  *nil_virt_internal_repr(Object *self);

Object *nil_construct();

#endif
