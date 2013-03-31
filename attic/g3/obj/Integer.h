#ifndef INTEGER_H
#define INTEGER_H

#include "obj/Object.h"

struct Integer : public Object
{
	Integer(Class *clas) : Object(clas) {}
	virtual ~Integer() {}
	virtual Type get_type() const { return TYPE_INTEGER; }

	static void install_methods(Class *integer_class);
};

#endif
