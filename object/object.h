#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>
#include <stddef.h>

struct string;
struct vm;
struct type;
struct symbol;
struct nil;
struct bool_;

typedef struct object
{
	struct type *type;
	int ref_cnt;
} object_t;

#define REF_CNT(obj) (((struct object *)obj)->ref_cnt)
#define INCR(obj) if (obj) { REF_CNT(obj) += 1; }
#define DECR(obj) if (obj) { REF_CNT(obj) -= 1; if (REF_CNT(obj) == 0) object_delete((struct object *)obj); obj = NULL; }
#define DECR0(obj) if (obj) { DECR(obj); obj = NULL; }

/* some public default slot functions */
object_t      *object_fn_allocate(struct type *type);
void           object_fn_deallocate(struct object *self);
void           object_fn_initialize(struct object *self);
void           object_fn_deinitialize(struct object *self);
wchar_t       *object_fn_repr(struct object *self);
size_t         object_fn_hash(struct object *self);

void object_delete(struct object *obj);

void objecttype_bootstrap(struct vm *itr);

#endif
