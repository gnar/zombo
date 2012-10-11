#ifndef TOOLS_H
#define TOOLS_H

#include "object/type.h"

#define NIL   ((Object*)vm_get()->i_nil)
#define TRUE  ((Object*)vm_get()->i_true)
#define FALSE ((Object*)vm_get()->i_false)

bool is_instance(struct object *obj, struct type *type);
struct string *repr(object_t *self);

#endif
