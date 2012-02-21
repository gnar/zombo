#ifndef ARRAY_H
#define ARRAY_H

#include "Object.h"

typedef struct Array
{
	Object base;
	Object **beg, **end, **cap;
} Array;

Array *array_allocate(struct Class *class, size_t len);

size_t array_capacity(Array *arr);
size_t array_length(Array *arr);
size_t array_resize(Array *arr, size_t len); /* returns: new capacity */

#endif
