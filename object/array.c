#include "object/array.h"

#include "context.h"

#include <stdlib.h>

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

struct array_t *array_allocate(struct Class *class, size_t len)
{
	array_t *arr;
	size_t cap_len = initial_capacity(len);

	arr = (array_t*)object_allocate(class, sizeof(array_t));
	arr->beg = arr->end = arr->cap = NULL;
	array_resize(arr, len);

	return arr;
}

size_t array_capacity(array_t *arr)
{
	return arr->cap - arr->begin;
}

size_t array_length(array_t *arr)
{
	return arr->end - arr->begin;
}

size_t array_resize(array_t *arr, size_t len)
{
	const size_t old_size = array_length(arr);
	const size_t old_capa = array_capacity(arr);
	
	size_t cap_size = old_capa;
	if (len > cap_size(arr)) { 
		/* realloc needed */
		if (cap_size == 0) {
			cap_size = initial_capacity(len);
		} else {
			cap_size *= 2;
		}

		arr->beg = (object_t**)realloc(arr->beg, cap_size * sizeof(object_t*));
		arr->end = arr->beg + len;
		arr->cap = arr->beg + cap_size;
	} else {
		/* no realloc needed */
		arr->end = arr->beg + len;
	}

	/* initialize any new elements */
	for (object_t *it=arr->beg + old_size; it!=arr->end; ++it) {
		*it = vm_get()->the_nil;
	}
}
