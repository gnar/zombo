#include "object/integer.h"
#include "object/object.h"
#include "object/object_fntab.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

static void integer_fn_on_destroy(struct Object *obj)
{
	printf("destroying an integer: %i\n", ((Integer*)obj)->value);
}

static FnTab integer_fntab = {
	&integer_fn_on_destroy,
};

void Integer_Initialize()
{
	FnTab_Define(TYPE_INTEGER, &integer_fntab);
}

Integer *Integer_New(int value)
{
	Integer *i = (Integer*)Object_Alloc(TYPE_INTEGER, sizeof(Integer));
	i->value = value;
	return i;
}
