#include "object/object.h"
#include "object/vm.h"
#include "object/nil.h"
#include "object/bool.h"
#include "object/string.h"
#include "object/symbol.h"
#include "object/type.h"
#include "object/map.h"
#include "object/function.h"
#include "object/integer.h"

#include "tools.h"

#include <stdlib.h>
#include <stdio.h>

void print_repr(object_t *obj)
{
	string_t *s = repr(obj);
	wprintf(L"%ls\n", s->s_chars);
	DECR(s);
}

void test0()
{
	object_t *x = (object_t*)integer_new(42);
	print_repr(x);
	DECR(x);

	//vm_t *v0 = vm_get();
	//print_repr((object_t*)vm_get());
	//DECR(v0);

	//symbol_t *s0 = symbol_new(L"some_symbol");
	//print_repr((object_t*)s0);
	//printf("aa: %i\n", s0->super.ref_cnt);
	//DECR(s0);

	/*wprintf(L"%ls\n", repr((object_t*)bool_new(false))->s_chars);
	wprintf(L"%ls\n", repr((object_t*)bool_new(true))->s_chars);
	wprintf(L"%ls\n", repr((object_t*)string_new(L"hallo"))->s_chars);
	wprintf(L"%ls\n", repr((object_t*)map_new())->s_chars);
	wprintf(L"%ls\n", repr((object_t*)function_new())->s_chars);*/
	/*type_t *der = type_new(L"mystr", vm_get()->string_type);*/
}

void test1()
{
	function_t *fn = function_new();
	print_repr((object_t*)fn);

	function_add_instr0(fn, I_NOP);
	function_add_instr (fn, I_PUSHI, 42);
	function_add_instr (fn, I_PUSHI, 58);
	function_add_instr0(fn, I_ADD);
	function_add_instr0(fn, I_RET);

	function_debug_print(fn);

	wprintf(L"---\n");
	DECR(fn);
}

int main()
{
	vm_create();
	test1();
	//test0();
	vm_shutdown(vm_get());
}
