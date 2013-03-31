#ifndef OBJECT_H
#define OBJECT_H

#include <stddef.h>
#include <stdint.h>

typedef struct Object_vtab Object_vtab;
typedef struct Object Object;

struct Object_vtab
{
	struct Object_vtab *super;
	struct Object *(*message)(Object *self, char *id);
	size_t (*get_size)(Object *self);
	char *(*internal_repr)(Object *self);
};

extern Object_vtab object_vtab;
Object *object_virt_message(Object *self, char *id);
size_t  object_virt_get_size(Object *self);
char   *object_virt_internal_repr(Object *self);

struct Object
{
	Object_vtab *vtab;
	struct Class *clas;
};

Object       *object_construct(struct Class *clas, size_t bytes);
Object_vtab  *object_get_vtab(Object *self);
struct Class *object_get_class(Object *self);
Object       *object_message(Object *self, char *id);
size_t        object_get_size(Object *self);
char         *object_internal_repr(Object *self);

#endif
