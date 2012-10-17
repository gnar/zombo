#ifndef INTEGER_H
#define INTEGER_H

#include "object/object.h"
#include "object/type.h"

typedef struct integer
{
	object_t super;
	int i;
} integer_t;

integer_t *integer_new(int i);
type_t *integertype_new();

#endif
