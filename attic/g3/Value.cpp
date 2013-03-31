#include "Value.h"
#include "Context.h"

#include "obj/Object.h"
#include "obj/Class.h"
#include "obj/String.h"
#include "obj/Native.h"

#include <cassert>

Value nil;

Value Value::get_class() const
{
	if (is_real_object()) {
		return obj->get_class();
	} else if (is_integer()) {
		return ctx->get_integer_class();
	} else if (is_nil()) {
		return ctx->get_nil_class();
	} else {
		assert(0);
	}
}

Value Value::message(const std::string &id) const
{
	if (is_real_object()) {
		return obj->message(id);
	} else {
		// Just look for methods in class
		Class *cls = static_cast<Class*>(get_class().obj);
		return cls->get_method(id);		
	}
}

Value Value::call(const std::string &id) const
{
	Value fn = message(id);
	if (fn.get_type() == TYPE_NATIVE) {
		static_cast<Native*>(fn.obj)->fn(*this, Value(), Value());
	} else {
		assert(0);
	}
}

std::ostream &operator<<(std::ostream &out, const Value &val)
{
	Value repr = val.call("repr");
	if (repr.get_type() == TYPE_STRING) {
		const std::string repr_str = static_cast<String*>(repr.obj)->str();
		out << repr_str;
	} else {
		out << "???";
	}
	return out;
}

Collectable **Value::gc_mark(Collectable **ptr)
{
	if (is_real_object() && (obj->gc_get_mark() == 0)) {
		obj->gc_set_mark(1);
		*ptr++ = obj;
	}
	return ptr;
}

Type Value::get_type_of_object() const
{
	return obj->get_type();
}
