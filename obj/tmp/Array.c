#include "Array.h"

#include "context.h"

#include <stdlib.h>

static object_message(struct Object *self, char *id);

static struct Object_vtab array_vtab = {
	object_message
};

static size_t initial_capacity(size_t len)
{
	if (len == 0) {
		assert(0);
		return 0;
	} else if (len <= 8) {
		return 8;
	} else {
		// get next power of two
		len -= 1;
		len |= len >>  1;
		len |= len >>  2;
		len |= len >>  4;
		len |= len >>  8;
		len |= len >> 16;
		len |= len >> 32;
		len += 1;
		return len;
	}
}

struct Array *array_allocate(struct Class *class, size_t len)
{
	Array *arr;
	size_t cap_len = initial_capacity(len);

	arr = (Array*)object_allocate(class, sizeof(Array));
	arr->beg = arr->end = arr->cap = NULL;
	array_resize(arr, len);

	return arr;
}

size_t array_capacity(Array *arr)
{
	return arr->cap - arr->begin;
}

size_t array_length(Array *arr)
{
	return arr->end - arr->begin;
}

size_t array_resize(Array *arr, size_t len)
{
	Object **it = NULL, **old_end = arr->end;

	size_t cap_size = array_capacity(arr);

	if (len > cap_size(arr)) { 
		/* realloc needed */

		if (cap_size == 0) {
			cap_size = initial_capacity(len);
		} else {
			cap_size *= 2;
		}

		arr->beg = (Object**)realloc(arr->beg, cap_size * sizeof(Object*));
		arr->end = arr->beg + len;
		arr->cap = arr->beg + cap_size;
	} else {
		/* no realloc needed */
		arr->end = arr->beg + len;
	}

	/* initialize any new elements */
	if (old_end) {
		for (it=old_end; it!=arr->end; ++it) {
			*it = ctx->nil;
		}
	}
}
