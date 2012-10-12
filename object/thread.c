#include "object/thread.h"
#include "object/type.h"
#include "object/vm.h"

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

void thread_execute(thread_t *thr)
{
	
}

/******************************************************************************/

static void thread_fn_initialize(object_t *obj)
{
	thread_t *thr = (thread_t*)obj;
	thr->frm = NULL;
}

static void thread_fn_deinitialize(object_t *obj)
{
	DECR0(thr->frame);
}

static wchar_t *thread_fn_repr(object_t *obj)
{
	return object_fn_repr(obj);
}

static size_t thread_fn_hash(object_t *obj)
{
	return 84; /*XXX*/
}
