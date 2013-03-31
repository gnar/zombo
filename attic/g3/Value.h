#ifndef VALUE_H
#define VALUE_H

#include <ostream>
#include <typeinfo>
#include <cassert>

#include "Type.h"
#include "gc/Collectable.h"

union Value
{
	Value() : obj(0) {}
	Value(class Object *obj) : obj(obj) {}

	enum BitMask : unsigned long {
		    MASK = 0x03ul,
		OBJ_FLAG = 0x00ul, 
		INT_FLAG = 0x01ul
	};

	inline Type get_type() const;

	unsigned long get_flags() const { return ul & MASK; }
	Object *get_obj() const { return obj; }

	bool is_nil()         const { return get_flags() == OBJ_FLAG && obj == 0; }
	bool is_real_object() const { return get_flags() == OBJ_FLAG && obj != 0; }
	bool is_integer()     const { return get_flags() == INT_FLAG; }

	operator bool() const { return !is_nil(); }
	bool operator !() const { return is_nil(); }
	bool identical(Value rhs) const { return obj == rhs.obj; }

	inline long extract_integer();
	inline void insert_integer(long l);

	// gc
	Collectable **gc_mark(Collectable **ptr);

	// wrappers
	Value get_class() const;
	Value message(const std::string &id) const;
	Value call(const std::string &id) const;

	// union fields
	struct Object *obj;
	unsigned long ul;

private:
	Type get_type_of_object() const;
};

std::ostream &operator<<(std::ostream &out, const Value &val);

typedef Value (*FnPtr)(Value self, Value env, Value args);

inline long Value::extract_integer()
{
	if (is_integer()) {
		unsigned long tmp = ul >> 2;
		return *reinterpret_cast<long*>(&tmp);
	} else {
		assert(0);
	}
}

inline void Value::insert_integer(long l)
{
	// TO DO: Check if |l| is small enough.
	ul = (*reinterpret_cast<unsigned long*>(&l) << 2) | 0x01ul;
}

inline Type Value::get_type() const
{
	if (is_integer()) {
		return TYPE_INTEGER;
	} else if (is_nil()) {
		return TYPE_NIL;
	} else {
		return get_type_of_object();
	}
}

#endif
