#include "obj/True.h"
#include "obj/Class.h"

#include "Context.h"

#define DEF(c, name) c->add_method(name, ctx->make_native([](Value self, Value env, Value args) -> Value {
#define END }));

void Nil::install_methods(Class *c)
{
	DEF(c, "repr")
		return ctx->make_string("nil");
	END

	DEF(c, "hash")
		return ctx->make_integer(4);
	END
}

void True::install_methods(Class *c)
{
	DEF(c, "repr")
		return ctx->make_string("true");
	END

	DEF(c, "hash")
		return ctx->make_integer(2);
	END
}

void False::install_methods(Class *c)
{
	DEF(c, "repr")
		return ctx->make_string("false");
	END

	DEF(c, "hash")
		return ctx->make_integer(0);
	END
}
