#include "obj/Integer.h"
#include "obj/Class.h"

#include "Context.h"

#include <cstdlib>
#include <sstream>

#define DEF(c, name) c->add_method(name, ctx->make_native([](Value self, Value env, Value args) -> Value {
#define END }));

void Integer::install_methods(Class *c)
{
	DEF(c, "repr")
		std::stringstream ss;
		if (self.is_integer()) {
			ss << self.extract_integer();
		} else {
			/*Class *c = static_cast<Class*>(self.obj);
			return ctx->make_string(c->get_class_name());*/
			ss << "<bigint>";
		}
		return ctx->make_string(ss.str());
	END

	DEF(c, "abs")
		if (self.is_integer()) {
			return ctx->make_integer(abs(self.extract_integer()));
		} else {
			return ctx->make_nil();
		}
	END

	DEF(c, "even?")
		if (self.is_integer()) {
			const bool is_even = abs(self.extract_integer()) % 2 == 0;
			return is_even ? ctx->make_true() : ctx->make_false();
		} else {
			return ctx->make_nil();
		}
	END

	DEF(c, "odd?")
		if (self.is_integer()) {
			const bool is_odd = abs(self.extract_integer()) % 2 == 1;
			return is_odd ? ctx->make_true() : ctx->make_false();
		} else {
			return ctx->make_nil();
		}
	END
}
