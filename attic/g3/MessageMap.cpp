#include "MessageMap.h"
#include "obj/Object.h"

#include <cassert>

bool MessageMap::set(const std::string &id, Value fn)
{
	if (!mapping) mapping.reset(new Mapping);

	mapping->insert(std::pair<std::string, Value>(id, fn));
}

bool MessageMap::get(const std::string &id, Value &fn) const
{
	if (!mapping) return false;

	Mapping::const_iterator it = mapping->find(id);
	if (it == mapping->end()) {
		return false;
	} else {
		fn = it->second;
		return true;
	}
}

bool MessageMap::del(const std::string &id)
{
	if (!mapping) return false;

	if (has(id)) {
		mapping->erase(id);
		return true;
	} else {
		return false;
	}
}

bool MessageMap::has(const std::string &id) const
{
	if (!mapping) return false;

	return mapping->find(id) != mapping->end();
}

Collectable **MessageMap::gc_mark(Collectable **b, Collectable **e)
{
	if (!mapping) return b;
	
	for (Mapping::iterator it=mapping->begin(); it!=mapping->end(); ++it) {
		if (e-b < 1) throw need_bigger_mark_list();
		b = it->second.gc_mark(b);
	}
	return b;
}

void MessageMap::gc_update_pointers()
{
	if (!mapping) return;

	assert(0);
	/*for (Mapping::const_iterator it=mapping->begin(); it!=mapping->end(); ++it) {
		Value fn = it->second;
		if (fn.is_real_object()) {
			fn.obj = (Object*)fn.obj->gc_get_forward_ptr();
		}
	}*/
}
