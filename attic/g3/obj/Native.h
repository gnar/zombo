#ifndef NATIVE_H
#define NATIVE_H

#include "obj/Instance.h"

class Native : public Instance
{
public:
	Native(Class *clas) : Instance(clas) {}
	virtual ~Native() {}
	virtual Type get_type() const { return TYPE_NATIVE; }

	Value (*fn)(Value self, Value env, Value args);

	static void install_methods(Class *native_class);
};

#endif
