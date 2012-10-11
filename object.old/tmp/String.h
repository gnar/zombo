#ifndef STRING_H
#define STRING_H

#include "Object.h"

typedef struct String
{
	Object base;

	size_t len;
	uint32_t str[0];
} String;

Object *string_alloc(size_t len);

#endif
