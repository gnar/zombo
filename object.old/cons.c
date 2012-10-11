#include "object/cons.h"
#include "object/object_fntab.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void cons_fn_on_destroy(struct Object *obj)
{
	Cons *cns = (Cons*)obj;
	DECREF(cns->car); cns->car = 0;
	DECREF(cns->cdr); cns->cdr = 0;
}

static FnTab cons_fntab = {
	&cons_fn_on_destroy,
};

void Cons_Initialize()
{
	FnTab_Define(TYPE_CONS, &cons_fntab);
}

Cons *Cons_New(Object *car, Object *cdr)
{
	Cons *cns = (Cons*)Object_Alloc(TYPE_CONS, sizeof(Cons));
	cns->car = car; INCREF(car);
	cns->cdr = cdr; INCREF(cdr);
	return cns;
}
