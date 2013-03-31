#ifndef STRING_H
#define STRING_H

#include "obj/Instance.h"

struct String : public Instance
{
public:
	String(Class *clas) : Instance(clas) {}
	virtual ~String() {}
	virtual Type get_type() const { return TYPE_STRING; }

	const std::string &str() const { return data; }
	std::string &str() { return data; }

	static void install_methods(Class *string_class);

private:
	std::string data;
};

#endif
