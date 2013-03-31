#include "obj/String.h"
#include "obj/Class.h"

#include "Context.h"

#define DEF(c, name) c->add_method(name, ctx->make_native([](Value self, Value env, Value args) -> Value {
#define END }));

void String::install_methods(Class *c)
{
	DEF(c, "length")
		String *s = static_cast<String*>(self.obj);
		return ctx->make_integer(s->str().size());
	END

	DEF(c, "repr")
		String *s = static_cast<String*>(self.obj);
		return ctx->make_string(s->str());
	END
}
