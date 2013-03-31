#ifndef INSTANCE_H
#define INSTANCE_H

#include "obj/Object.h"

#include "MessageMap.h"

class Instance : public Object
{
public:
	Instance(Class *clas) : Object(clas) {}
	virtual ~Instance() {}
	virtual Type get_type() const { return TYPE_INSTANCE; }

	Value get_inst_meth(const std::string &id) const;
	Value set_inst_meth(const std::string &id, Value fn);
	virtual Value message(const std::string &id) const;

	virtual Collectable **gc_mark(Collectable **lbeg, Collectable **lend);
	virtual void gc_update_pointers();

private:
	MessageMap inst_meths;
};

#endif
