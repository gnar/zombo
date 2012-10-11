#ifndef INTEGER_H
#define INTEGER_H

#include "object/object.h"

typedef struct Integer {
	Object obj;
	int value;
} Integer;

void Integer_Initialize();

Integer *Integer_New(int value);

#endif
