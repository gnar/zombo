#ifndef MESSAGE_MAP_H
#define MESSAGE_MAP_H

#include "Value.h"

#include <memory>
#include <string>
#include <map>

class Object;
class Collectable;

class MessageMap
{
public:
	bool set(const std::string &id, Value fn);
	bool get(const std::string &id, Value &fn) const;
	bool del(const std::string &id);
	bool has(const std::string &id) const;

	Collectable **gc_mark(Collectable **lbeg, Collectable **lend);
	void gc_update_pointers();

private:
	typedef std::map<std::string, Value> Mapping;
	std::unique_ptr<Mapping> mapping;
};

#endif
