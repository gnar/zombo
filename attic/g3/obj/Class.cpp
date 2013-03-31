#include "obj/Class.h"
#include "obj/Object.h"
#include "obj/String.h"
#include "obj/Native.h"

#include "Context.h"

#include <iostream>
using namespace std;

Class::Class(Class *clas) : Instance(clas), alloc_type(TYPE_OBJECT)
{
}

Value Class::get_method(const std::string &id) const
{
	Value fn; const bool found = methods.get(id, fn);
	if (found) {
		return fn;
	} else if (base) {
		return base->get_method(id);
	} else {
		return Value();
	}
}

void Class::add_method(const std::string &id, Value fn)
{
	methods.set(id, fn);
}

Value Class::make_instance()
{
	switch (alloc_type) {
		case TYPE_OBJECT:   return new Object(this); // should never be called. (?)
		case TYPE_INSTANCE: return new Instance(this);
		case TYPE_NATIVE:   return new Native(this);
		case TYPE_STRING:   return new String(this);
		case TYPE_CLASS:    return new Class(this);
		case TYPE_NIL:      return Value();
		case TYPE_TRUE:     return ctx->make_true();
		case TYPE_FALSE:    return ctx->make_false();
		case TYPE_INTEGER:  return Value(0l);
		default: assert(0);
	}
}

void Class::set_base(Class *base, Type alloc_type)
{
	this->base = base;
	this->alloc_type = alloc_type;
}

Collectable **Class::gc_mark(Collectable **b, Collectable **e)
{
	b = Instance::gc_mark(b, e);
	b = methods.gc_mark(b, e);

	// Hmm:
	if (e-b < 1) throw need_bigger_mark_list();
	if (base && base->gc_get_mark() == 0) {
		base->gc_set_mark(1);
		*b++ = base;
	}

	return b;
}

void Class::gc_update_pointers()
{
	assert(0);
	/*Instance::gc_update_pointers();
	methods.gc_update_pointers();
	if (base.is_real_object()) base.obj = (Object*)base.obj->gc_get_forward_ptr();*/
}

#define DEF(c, name) c->add_method(name, ctx->make_native([](Value self, Value env, Value args) -> Value {
#define END }));

void Class::install_methods(Class *c)
{
	DEF(c, "repr")
		Class *c = static_cast<Class*>(self.obj);
		return ctx->make_string(c->get_name());
	END

	/*class_class->add_method("new",     native(class_methods::new_));
	class_class->add_method("base",    native(class_methods::base));*/
}
