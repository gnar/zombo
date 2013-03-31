#ifndef TYPE_H
#define TYPE_H

enum Type : unsigned int
{
	TYPE_OBJECT=0,
	TYPE_INSTANCE,
	TYPE_CLASS,
	TYPE_NIL,
	TYPE_TRUE,
	TYPE_FALSE,
	TYPE_INTEGER,
	TYPE_NATIVE,
	TYPE_STRING,
};

#endif