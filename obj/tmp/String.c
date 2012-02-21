#include "String.h"

#include <stdlib.h>

Object *string_alloc(size_t len)
{
	String *s = (String*)malloc(sizeof(String) + len * sizeof(uint32_t));

	s->len = len;
	for (size_t i=0; i<len; ++i) s->str[i] = ' ';

	return a;
}

