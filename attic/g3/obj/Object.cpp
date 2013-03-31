#include "obj/Object.h"
#include "obj/Class.h"

#include "Context.h"

#include <cassert>
#include <sstream>

Value Object::message(const std::string &id) const
{
	// Search in methods of class
	return get_class()->get_method(id);
}

Collectable **Object::gc_mark(Collectable **b, Collectable **e)
{
	if (e-b < 1) throw need_bigger_mark_list();

	if (clas && clas->gc_get_mark() == 0) {
		clas->gc_set_mark(1);
		*b++ = clas;
	}

	return b;
}

void Object::gc_update_pointers()
{
	assert(0);
	/*if (clas.is_real_object()) {
		clas.obj = (Object*)clas.obj->gc_get_forward_ptr();
	}*/
}

#define DEF(c, name) c->add_method(name, ctx->make_native([](Value self, Value env, Value args) -> Value {
#define END }));

void Object::install_methods(Class *c)
{
	DEF(c, "repr")
		std::stringstream ss;
		ss << "<Object:" << (void*)self.obj << ">";
		return ctx->make_string(ss.str());
	END

	DEF(c, "class")
		return self.get_class();
	END

	DEF(c, "hash")
		return ctx->make_integer(*reinterpret_cast<long*>(self.obj) % 0xfffffff0);
	END
}
