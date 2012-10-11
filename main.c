#include "object/object.h"
#include "object/vm.h"
#include "object/nil.h"
#include "object/bool.h"
#include "object/string.h"
#include "object/symbol.h"
#include "object/type.h"
#include "object/map.h"
#include "object/function.h"

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

int main()
{
	vm_create();
	vm_shutdown(vm_get());
}
