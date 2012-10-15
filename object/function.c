#include "object/function.h"

#include "object/vm.h"

#include "tools.h"

#include <stdlib.h>

static void function_fn_initialize(object_t *);
static void function_fn_deinitialize(object_t *);
static size_t function_fn_hash(object_t *);
static wchar_t *function_fn_repr(object_t *);

function_t *function_new()
{
	type_t *fn_type = vm_get()->function_type;
	function_t *fn = (function_t*)type_allocate(fn_type);
	type_initialize(fn_type, (object_t*)fn);
	return fn;
}

type_t *functiontype_new()
{
	type_t *st = type_new(L"func", vm_get()->object_type);
	st->basicsize = sizeof(struct function);
	st->fn_initialize   = &function_fn_initialize;
	st->fn_deinitialize = &function_fn_deinitialize;
	st->fn_repr         = &function_fn_repr;
	st->fn_hash         = &function_fn_hash;
	return st;
}

bool function_check(object_t *self)
{
	return is_instance(self, vm_get()->function_type);
}

void function_add_instr(function_t *fn, int opcode, int arg)
{
	const int pos = fn->num_instr;

	/* make space */
	fn->num_instr += 1;
	if (fn->num_instr >= fn->instr_size) {
		fn->instr_size *= 2;
		fn->instr      = (int*)realloc(fn->instr     , sizeof(int) * fn->instr_size);
		fn->instr_args = (int*)realloc(fn->instr_args, sizeof(int) * fn->instr_size);
	}

	/* add instruction */
	fn->instr[pos] = opcode;
	fn->instr_args[pos] = arg;
}

/********************************************************************************/

static const size_t DEFAULT_INSTR_SIZE = 32;

static void function_fn_initialize(object_t *obj)
{
	function_t *fn = (function_t*)obj;

	fn->num_instr = 0;
	fn->instr_size = DEFAULT_INSTR_SIZE;
	fn->instr = (int*)malloc(sizeof(int) * DEFAULT_INSTR_SIZE);
	fn->instr_args = (int*)malloc(sizeof(int) * DEFAULT_INSTR_SIZE);
	fn->stack_size = 0;
	fn->num_constants = 0;
	fn->constants = NULL;
	fn->num_locals = 0;
	fn->locals = NULL;
}

static void function_fn_deinitialize(object_t *obj)
{
	function_t *fn = (function_t*)obj;

	free(fn->instr);
	free(fn->instr_args);

	DECR0(fn->constants);
	DECR0(fn->locals);
}

static wchar_t *function_fn_repr(object_t *obj)
{
	return object_fn_repr(obj);
}

static size_t function_fn_hash(object_t *self)
{
	return 24; /*XXX*/
}
