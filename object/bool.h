#ifndef BOOL_H
#define BOOL_H

#include <stdbool.h>

#include "object/object.h"

typedef struct bool_ {
	struct object super;
	bool value;
} bool_t;

bool_t *bool_new(bool value);
struct type *booltype_new();

bool bool_check(object_t *self);
bool bool_is_true(bool_t *self);

#endif
