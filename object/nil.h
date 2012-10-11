#ifndef NIL_H
#define NIL_H

#include <stdbool.h>

#include "object/object.h"

typedef struct nil {
	struct object super;
} nil_t;

void nil_bootstrap(struct vm *itp);

nil_t *nil_new();
struct type *niltype_new();

bool nil_check(object_t *self);

#endif
