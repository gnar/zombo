#include "Value.h"
#include "Context.h"

#include "obj/Object.h"
#include "obj/Class.h"
#include "obj/Instance.h"

#include "gc/Collector.h"

#include <typeinfo>
#include <iostream>
using namespace std;

int main()
{
	cout << sizeof(Value) << endl;
	cout << sizeof(Object) << endl;
	cout << sizeof(Instance) << endl;
	cout << sizeof(Class) << endl;

	ctx = new Context;
	ctx->create_classes();

	Value s0 = ctx->make_string("Hallo");
	cout << s0.call("length") << endl;
	cout << s0.call("hash") << endl;
	cout << ctx->get_string_class().call("repr").call("length") << endl;

	Value i0 = ctx->make_integer(123);
	cout << i0.call("even?") << endl;
	cout << i0.call("odd?") << endl;

	/*Collectable *mark_list[1024];
	Collectable *c = ctx->get_string_class().obj;
	Collectable **pos = c->gc_mark(mark_list, mark_list+1024);
	for (Collectable **it = mark_list; it != pos; ++it) {
		Collectable *obj = *it;
		cout << (void*)obj << " " << typeid(*obj).name() << endl;
	}*/
	
	Collector *gc = new Collector;
	gc->add_root(ctx->get_string_class().obj);
	gc->breadth_first_mark();

	delete gc;
	delete ctx;
}
