#include "obj/Instance.h"
#include "obj/Class.h"

#include "Context.h"

Value Instance::get_inst_meth(const std::string &id) const
{
	Value fn; 
	if (inst_meths.get(id, fn)) return fn;

	return Value();
}

Value Instance::set_inst_meth(const std::string &id, Value fn)
{
	inst_meths.set(id, fn);
}

Value Instance::message(const std::string &id) const
{
	// I. Search for instance methods
	Value fn; 
	if (inst_meths.get(id, fn)) return fn;

	// II. Search in methods of class (this is the behavior of Object::message).
	return Object::message(id);
}

Collectable **Instance::gc_mark(Collectable **b, Collectable **e)
{
	b = Object::gc_mark(b, e);
	b = inst_meths.gc_mark(b, e);
	return b;
}

void Instance::gc_update_pointers()
{
	Object::gc_update_pointers();
	inst_meths.gc_update_pointers();
}
