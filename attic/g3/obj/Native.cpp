#include "obj/Native.h"
#include "obj/Class.h"

#include "Context.h"

#include <sstream>

#define DEF(c, name) c->add_method(name, ctx->make_native([](Value self, Value env, Value args) -> Value {
#define END }));

void Native::install_methods(Class *c)
{
	DEF(c, "repr")
		Native *n = static_cast<Native*>(self.obj);
		std::stringstream ss;
		ss << "<Native:" << (void*)self.obj << "/" << (void*)n->fn << ">";
		return ctx->make_string(ss.str());
	END
}
