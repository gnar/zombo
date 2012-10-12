#ifndef ZB_TYPE_H
#define ZB_TYPE_H

#include "object.h"

#include <wchar.h>

typedef struct type
{
	struct object super;

	wchar_t *name; /* the type name */
	struct type *base; /* base type */
	size_t basicsize;
	
	object_t      *(*fn_allocate)    (struct type *type);
	void           (*fn_deallocate)  (object_t *self);
	void           (*fn_initialize)  (object_t *self);
	void           (*fn_deinitialize)(object_t *self);

	size_t         (*fn_hash)        (object_t *self);
	wchar_t       *(*fn_repr)        (object_t *self);

	/* message map (for methods of class) */
} type_t;

type_t *type_new(const wchar_t *name, type_t *base); /* create a new type object */

/* alloc memory (and set super.type) */
object_t *type_allocate(type_t *type);
void    type_deallocate(type_t *type, object_t *obj);

/* initialize object */
void type_initialize(type_t *type, object_t *obj); /* initialize an object */
void type_deinitialize(type_t *type, object_t *obj);

#endif
