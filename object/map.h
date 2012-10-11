#ifndef MAP_H
#define MAP_H

#include <stddef.h>
#include "object/object.h"

typedef struct map
{
	object_t super;
	size_t i, n, mask; // i=3,4,5,...; n=2**i; mask=(2**i)-1
	size_t filled; /* how many slots are filled with a key/value entry? */
	struct map_slot *slots; // array of size 'n'
} map_t;

map_t *map_new();

struct type *maptype_new();
bool map_check(struct object *self);

#endif
