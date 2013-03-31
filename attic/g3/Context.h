#ifndef CONTEXT_H
#define CONTEXT_H

#include "Value.h"

struct Context
{
public:
	Value get_object_class()  const { return  object_class; }
	Value get_class_class()   const { return   class_class; }
	Value get_nil_class()     const { return     nil_class; }
	Value get_integer_class() const { return integer_class; }
	Value get_native_class()  const { return  native_class; }
	Value get_string_class()  const { return  string_class; }
	Value get_true_class()    const { return    true_class; }
	Value get_false_class()   const { return   false_class; }

	void create_classes();

	Value make_nil() { return nil_obj; }
	Value make_true() { return true_obj; }
	Value make_false() { return false_obj; }
	Value make_native(FnPtr fn);
	Value make_string(const std::string &init = "");
	Value make_integer(long l);

private:
	Value true_obj, false_obj, nil_obj;

	Value object_class, class_class, nil_class;
	Value true_class, false_class;
	Value string_class;
	Value integer_class;
	Value native_class;
};

extern Context *ctx;

#endif
