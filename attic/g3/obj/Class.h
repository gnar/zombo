#ifndef CLASS_H
#define CLASS_H

#include "obj/Instance.h"
#include "MessageMap.h"

#include <string>

class Class : public Instance
{
public:
	Class(Class *clas);
	virtual ~Class() {}
	virtual Type get_type() const { return TYPE_CLASS; }

	const std::string &get_name() const { return name; }
	void set_name(const std::string &n) { name = n; }

	Class *get_base() const { return base; }
	void set_base(Class *base, Type alloc_type);

	Value get_method(const std::string &id) const;
	void add_method(const std::string &id, Value fn);

	Value make_instance();

	// gc interface
	virtual Collectable **gc_mark(Collectable **lbeg, Collectable **lend);
	virtual void gc_update_pointers();

private:
	std::string name;
	Type alloc_type;
	MessageMap methods; // methods for objects of this class.
	Class *base;
	
public:
	static void install_methods(Class *class_class);
};

#endif
