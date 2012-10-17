#ifndef INTEGER_H
#define INTEGER_H

#include "object/object.h"

typedef struct integer
{
	object_t super;
	int i;
} integer_t;

object_t *integer_new(int i);

#endif
