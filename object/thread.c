#include "object/thread.h"
#include "object/type.h"
#include "object/vm.h"
#include "object/integer.h"

#include <assert.h>

static void thread_fn_initialize(object_t *self_o);
static void thread_fn_deinitialize(object_t *self_o);
static wchar_t *thread_fn_repr(object_t *self_o);
static size_t thread_fn_hash(object_t *self);

/******************************************************************************/

struct type *threadtype_new()
{
	type_t *tt = type_new(L"thread", vm_get()->object_type);
	tt->basicsize = sizeof(struct thread);
	tt->fn_initialize   = &thread_fn_initialize;
	tt->fn_deinitialize = &thread_fn_deinitialize;
	tt->fn_repr         = &thread_fn_repr;
	tt->fn_hash         = &thread_fn_hash;
	return tt;
}

/******************************************************************************/

thread_t *thread_new(closure_t *clos)
{
	type_t *thr_type = vm_get()->thread_type;
	thread_t *thr = (thread_t*)type_allocate(thr_type);
	type_initialize(thr_type, (object_t*)thr);
	return thr;
}

static inline void stack_push(frame_t *frm, object_t *value)
{
	frm->sp += 1;
	frm->stack[frm->sp] = value;
}

static inline object_t *stack_pop(frame_t *frm)
{
	object_t *value = frm->stack[frm->sp];
	frm->sp -= 1;
	return value;
}

void thread_execute(thread_t *thr)
{
	frame_t *frm = thr->frm;
	closure_t *clos = frm->clos;
	function_t *func = clos->func;

	object_t *tmp;

	while (true) {
		const size_t ip = frm->ip;
		int opcode = func->instr[ip];
		switch (opcode) {
			case I_NOP: 
				goto next;

			case I_PUSHI:
				stack_push(frm, (object_t*)integer_new(func->instr_args[ip]));
				goto next;
				
			case I_PUSHC:
				tmp = func->constants[func->instr_args[ip]]; INCR(tmp);
				stack_push(frm, tmp);
				goto next;

			case I_PUSHL:
				assert(0);

			case I_RET:
				assert(0);

			case I_POP:
				assert(0);

			case I_POPL:
				assert(0);

			case I_NEG:
				assert(0);

			case I_ADD:
				assert(0);

			case I_SUB:
				assert(0);

			case I_MUL:
				assert(0);

			case I_DIV:
				assert(0);

			case I_APPLY:
				assert(0);

			case I_PRINT:
				assert(0);

			default: assert(0);
		}
next:		
		frm->ip += 1;
noincr:
		;
	}
}

/******************************************************************************/

static void thread_fn_initialize(object_t *obj)
{
	thread_t *thr = (thread_t*)obj;
	thr->frm = NULL;
}

static void thread_fn_deinitialize(object_t *obj)
{
	thread_t *thr = (thread_t*)obj;
	DECR0(thr->frm);
}

static wchar_t *thread_fn_repr(object_t *obj)
{
	return object_fn_repr(obj);
}

static size_t thread_fn_hash(object_t *obj)
{
	return 84; /*XXX*/
}
