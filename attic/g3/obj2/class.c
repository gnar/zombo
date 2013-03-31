#include "class.h"
#include "instance.h"
#include "nil.h"

#include <string.h>
#include <assert.h>

Class *class_construct(Class *class, Class *base, char *name, size_t bytes)
{
	Class *c = (Class*)instance_construct(class, bytes);
	c->super.super.vtab = (Object_vtab*)&class_vtab;
	c->name = name ? strdup(name) : 0;
	c->base = base;
	return c;
}

Class_vtab *class_get_vtab(Class *self)
{
	return (Class_vtab*)(self->super.super.vtab);
}

Object *class_construct_object(Class *self)
{
	return class_get_vtab(self)->construct_object(self);
}

Object *class_get_method(Class *self, char *id) /* look up class method */
{
	assert(0);
}

void class_set_method(Class *self, char *id, struct Object *fn)
{
	assert(0);
}

void class_del_method(Class *self, char *id)
{
	assert(0);
}

// Virtual methods ////////////////////////////////////////

Class_vtab class_vtab = {
	{
		&instance_vtab,
		instance_virt_message,
		class_virt_get_size,
		class_virt_internal_repr,
	},
	class_virt_construct_object
};

size_t class_virt_get_size(Object *self0)
{
	return sizeof(Class);
}

Object *class_virt_construct_object(Class *self)
{
	Instance *obj = instance_construct(self, sizeof(Instance));
	return (Object*)obj;
}

char *class_virt_internal_repr(Object *self0)
{
	Class *self = (Class*)self0;
	return strdup(self->name);
}

//////////////////////// Special classes ////////////////////////////

static Class *class_construct_special(
	Class *clas, Class *base, char *name, 
	Class_vtab *vtab,
	Object *(*construct_object_fn)(Class *self)
) {
	// Setup vtab, replace construct_object entry.
	memcpy(vtab, &class_vtab, sizeof(Class_vtab));
	vtab->construct_object = construct_object_fn;
	
	// Construct class, install unique vtab.
	Class *c = class_construct(clas, base, name, sizeof(Class));
	c->super.super.vtab = (Object_vtab*)vtab;
	return c;
}

// VTables for special classes.
static Class_vtab nil_class_vtab;
static Class_vtab true_class_vtab;
static Class_vtab false_class_vtab;
static Class_vtab object_class_vtab;
static Class_vtab class_class_vtab;

// The Nil class //////////////////////////////////////////

static Object *nil_construct_object(Class *nil_class)
{
	return nil_construct();
}

Class *class_construct_nil_class(Class *clas, Class *base)
{
	return class_construct_special(clas, base, "Nil", &nil_class_vtab, nil_construct_object);
}

// The Object class //////////////////////////////////////////

static Object *object_construct_object(Class *object_class)
{
	assert("Can't construct instance of class Object!" && 0);
}

Class *class_construct_object_class(Class *clas, Class *base)
{
	return class_construct_special(clas, base, "Object", &object_class_vtab, object_construct_object);
}

// The Class class //////////////////////////////////////////

static Object *class_construct_class(Class *class_class)
{
	return (Object*)class_construct(class_class, 0, 0, sizeof(Class)); 
}

Class *class_construct_class_class(Class *clas, Class *base)
{
	return class_construct_special(clas, base, "Class", &class_class_vtab, class_construct_class);
}

// The True class //////////////////////////////////////////

static Object *true_construct_object(Class *true_class)
{
	assert(0);
}

Class *class_construct_true_class(Class *clas, Class *base)
{
	return class_construct_special(clas, base, "True", &true_class_vtab, true_construct_object);
}

// The False class ////////////////////////////////////////

static Object *false_construct_object(Class *false_class)
{
	assert(0);
}

Class *class_construct_false_class(Class *clas, Class *base)
{
	return class_construct_special(clas, base, "False", &false_class_vtab, false_construct_object);
}
