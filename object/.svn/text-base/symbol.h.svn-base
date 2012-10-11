#ifndef ZB_SYMBOL_H
#define ZB_SYMBOL_H

#include "object/object.h"

#include <wchar.h>

typedef struct symbol {
	struct object super;

	size_t s_len;
	wchar_t *s_chars;
} symbol_t;

symbol_t *symbol_new(const wchar_t *chars);
symbol_t *symbol_new_from_string(struct string *str); /* TODO */

struct type *symboltype_new();
bool symbol_check(object_t *self);

#endif
