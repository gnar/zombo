#ifndef CONS_H
#define CONS_H

#include "object/object.h"

typedef struct Cons {
	Object obj;
	Object *car, *cdr;
} Cons;

void Cons_Initialize();

Cons *Cons_New(Object *car, Object *cdr);

#endif
