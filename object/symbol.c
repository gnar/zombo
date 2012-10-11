#include "object/symbol.h"
#include "object/type.h"
#include "object/vm.h"
#include "object/string.h"

#include "tools.h"

#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

static void symbol_fn_initialize(object_t *self_o)
{
	symbol_t *self = (symbol_t*)self_o;
	self->s_len = 0;
	self->s_chars = NULL;
}

static void symbol_fn_deinitialize(object_t *self_o)
{
	symbol_t *self = (symbol_t*)self_o;
	free(self->s_chars);
	self->s_len = 0;
	self->s_chars = NULL;
}

static wchar_t *symbol_fn_repr(object_t *self_o)
{
	symbol_t *self = (symbol_t*)self_o;

	wchar_t *repr = (wchar_t*)malloc((self->s_len+2) * sizeof(wchar_t)); /* add space for '\0' and ':' */
	repr[0] = L':';
	wcsncpy(&repr[1], self->s_chars, self->s_len+1);
	
	return repr;
}

static size_t symbol_fn_hash(object_t *self_o)
{
	return 54;
} 

type_t *symboltype_new()
{
	type_t *st = type_new(L"symbol", vm_get()->object_type);
	st->basicsize = sizeof(struct symbol);
	st->fn_initialize   = &symbol_fn_initialize;
	st->fn_deinitialize = &symbol_fn_deinitialize;
	st->fn_repr         = &symbol_fn_repr;
	st->fn_hash         = &symbol_fn_hash;
	return st;
}

/*****************************************************************************/

symbol_t *symbol_new(const wchar_t *chars)
{
	symbol_t *self = (symbol_t*)type_allocate(vm_get()->symbol_type);
	type_initialize(vm_get()->symbol_type, (object_t*)self);

	free(self->s_chars);
	self->s_len = wcslen(chars);
	self->s_chars = wcsdup(chars);
	return self;
}

symbol_t *Symbol_NewFromCharArray(const char *chars)
{
	symbol_t *self;
	const int len = strlen(chars) + 1;
	wchar_t *wchars = (wchar_t*)malloc(len * sizeof(wchar_t));
	
	mbstowcs(wchars, chars, len);
	self = symbol_new(wchars);
	free(wchars);

	return self;
}

bool symbol_check(object_t *self)
{
	return is_instance(self, vm_get()->string_type);
}
