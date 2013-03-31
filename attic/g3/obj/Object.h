#ifndef OBJECT_H
#define OBJECT_H

#include "gc/Collectable.h"

#include "Value.h"
#include <cstddef>

class Class;

class Object : public Collectable
{
public:
	Object(Class *clas) : clas(clas) {}
	virtual ~Object() {}
	virtual Type get_type() const { return TYPE_OBJECT; }
	Class *get_class() const { return clas; }

	virtual Value message(const std::string &id) const;

	// GC Interface.
	virtual Collectable **gc_mark(Collectable **b, Collectable **e);
	virtual void gc_update_pointers();

	// Create Object class methods.
	static void install_methods(Class *object_class);

private:
	Class *clas;

	friend class Context; // needs to patch 'clas' directly.
};

#endif
