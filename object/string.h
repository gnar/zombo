#ifndef ZB_STRING_H
#define ZB_STRING_H

#include "object.h"

#include <stdbool.h>
#include <wchar.h>

typedef struct string {
	struct object super;

	size_t length;
	wchar_t *s_chars;
} string_t;

string_t *string_new(const wchar_t *chars);
string_t *string_new_from_char_array(const char *chars);

struct type *stringtype_new();
bool string_check(object_t *self);

#endif
