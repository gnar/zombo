#include "object/string.h"
#include "object/type.h"
#include "object/vm.h"

#include "tools.h"

#include <stdlib.h>
#include <wchar.h>
#include <string.h>

static void string_fn_initialize(object_t *self_o)
{
	string_t *self = (string_t*)self_o;
	self->length = 0;
	self->s_chars = NULL;
}

static void string_fn_deinitialize(object_t *self_o)
{
	string_t *self = (string_t*)self_o;
	free(self->s_chars);
	self->length = 0;
	self->s_chars = NULL;
}

static wchar_t *string_fn_repr(object_t *self_o)
{
	return wcsdup(((string_t*)self_o)->s_chars);
}

static size_t string_fn_hash(object_t *self)
{
	return 84; /*XXX*/
}

type_t *stringtype_new()
{
	type_t *st = type_new(L"string", vm_get()->object_type);
	st->basicsize = sizeof(struct string);
	st->fn_initialize   = &string_fn_initialize;
	st->fn_deinitialize = &string_fn_deinitialize;
	st->fn_repr         = &string_fn_repr;
	st->fn_hash         = &string_fn_hash;
	return st;
}

/*****************************************************************************/

string_t *string_new(const wchar_t *chars)
{
	type_t *str_type = vm_get()->string_type;
	string_t *self = (string_t*)type_allocate(str_type);
	type_initialize(str_type, (object_t*)self);

	free(self->s_chars);
	self->length = wcslen(chars);
	self->s_chars = wcsdup(chars);
	return self;
}

string_t *string_new_from_char_array(const char *chars)
{
	string_t *self;
	const size_t len = 0; //strlen(chars) + 1;
	wchar_t *wchars = (wchar_t*)malloc(len * sizeof(wchar_t));
	
	mbstowcs(wchars, chars, len);
	self = string_new(wchars);
	free(wchars);

	return self;
}

bool string_check(object_t *self)
{
	return is_instance(self, vm_get()->string_type);
}
