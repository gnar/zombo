#ifndef ARRAY_H
#define ARRAY_H

#include "object/object.h"

#include <stddef.h>

typedef struct array {
	object_t super;
	object_t **beg, **end, **cap;
} array_t;

size_t array_capacity(array_t *arr);
size_t array_length(array_t *arr);
size_t array_resize(array_t *arr, size_t len); /* returns new capacity */

#endif
