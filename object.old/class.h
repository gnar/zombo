#ifndef CLASS_H
#define CLASS_H

#include "instance.h"

typedef struct Class_vtab Class_vtab;
typedef struct Class Class;

struct Class_vtab
{
	Object_vtab super;
	Object *(*construct_object)(Class *self);
};

extern Class_vtab class_vtab;
size_t class_virt_get_size(Object *self0);
char *class_virt_internal_repr(Object *self);
Object *class_virt_construct_object(Class *self);

struct Class
{
	Instance super;
	char *name; /* name of the class */
	Class *base; /* base class of this class */
};

Class *class_construct(Class *clas, Class *base, char *name, size_t bytes);
Class *class_construct_nil_class(Class *clas, Class *base);
Class *class_construct_object_class(Class *clas, Class *base);
Class *class_construct_class_class(Class *clas, Class *base);
Class *class_construct_true_class(Class *clas, Class *base);
Class *class_construct_false_class(Class *clas, Class *base);

Object *class_construct_object(Class *self);
Class_vtab *class_get_vtab(Class *self);
Object *class_get_method(Class *self, char *id); /* look up class method */
void    class_set_method(Class *self, char *id, struct Object *fn);
void    class_del_method(Class *self, char *id);

#endif
