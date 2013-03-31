#ifndef TRUE_H
#define TRUE_H

#include "Object.h"

struct Nil : public Object
{
	Nil(Class *clas) : Object(clas) {}
	virtual ~Nil() {}
	virtual Type get_type() const { return TYPE_NIL; }

	static void install_methods(Class *nil_class);
};

struct True : public Object
{
	True(Class *clas) : Object(clas) {}
	virtual ~True() {}
	virtual Type get_type() const { return TYPE_TRUE; }

	static void install_methods(Class *true_class);
};

struct False : public Object
{
	False(Class *clas) : Object(clas) {}
	virtual ~False() {}
	virtual Type get_type() const { return TYPE_FALSE; }

	static void install_methods(Class *false_class);
};

#endif
