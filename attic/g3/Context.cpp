#include "Context.h"
#include "Value.h"

#include "obj/Class.h"
#include "obj/String.h"
#include "obj/Native.h"
#include "obj/Integer.h"
#include "obj/True.h"

#include <sstream>

Context *ctx = 0;

void Context::create_classes()
{
	// Construct classes
	Class *obc = new Class( 0 ); obc->set_name("Object"); 
	Class *clc = new Class( 0 ); clc->set_name("Class"); clc->clas = obc->clas = clc;
	Class *nic = new Class(clc); nic->set_name("Nil");   
	Class *inc = new Class(clc); inc->set_name("Integer");
	Class *nac = new Class(clc); nac->set_name("Native");      
	Class *stc = new Class(clc); stc->set_name("String");      
	Class *trc = new Class(clc); trc->set_name("True");
	Class *fac = new Class(clc); fac->set_name("False");

	obc->set_base( 0 , TYPE_OBJECT);
	clc->set_base(obc, TYPE_CLASS);
	nic->set_base(obc, TYPE_NIL);
	inc->set_base(obc, TYPE_INTEGER);
	nac->set_base(obc, TYPE_NATIVE);
	stc->set_base(obc, TYPE_STRING);
	trc->set_base(obc, TYPE_TRUE);
	fac->set_base(obc, TYPE_FALSE);

	// Add classes
	this->object_class = obc;
	this->class_class = clc;
	this->nil_class = nic;
	this->integer_class = inc;
	this->native_class = nac;
	this->string_class = stc;
	this->true_class = trc;
	this->false_class = fac;

	// Add objects
	this->true_obj = Value(new True(trc));
	this->false_obj = Value(new False(fac));
	this->nil_obj = Value((Object*)0);

	// Finally (!), add methods to classes.
	Object::install_methods(obc);
	True::install_methods(trc);
	False::install_methods(fac);
	Nil::install_methods(nic);
	Class::install_methods(clc);
	Native::install_methods(nac);
	String::install_methods(stc);
	Integer::install_methods(inc);
}

Value Context::make_native(FnPtr fn)
{
	Value nat = static_cast<Class*>(get_native_class().obj)->make_instance();
	static_cast<Native*>(nat.obj)->fn = fn;
	return nat;
}

Value Context::make_string(const std::string &init)
{
	Value str = static_cast<Class*>(get_string_class().obj)->make_instance();
	static_cast<String*>(str.obj)->str() = init;
	return str;
}

Value Context::make_integer(long l)
{
	Value i;
	i.insert_integer(l);
	return i;
}
