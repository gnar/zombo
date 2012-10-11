#include "object/map.h"
#include "object/type.h"
#include "object/vm.h"
#include "object/nil.h"
#include "object/string.h"

#include "tools.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

static void map_fn_initialize(object_t *self_o);
static void map_fn_deinitialize(object_t *self_o);
static wchar_t *map_fn_repr(object_t *self_o);
static size_t map_fn_hash(object_t *self);

static void map_initialize(struct map *map);
static void map_deinitialize(struct map *map);
static void map_resize(struct map *map, size_t i);

map_t *map_new()
{
	object_t *map = type_allocate(vm_get()->map_type);
	type_initialize(vm_get()->map_type, map);
	return (map_t*)map;
}

type_t *maptype_new()
{
	type_t *mt = type_new(L"map", vm_get()->object_type);
	mt->basicsize = sizeof(struct map);
	mt->fn_initialize   = &map_fn_initialize;
	mt->fn_deinitialize = &map_fn_deinitialize;
	mt->fn_repr         = &map_fn_repr;
	mt->fn_hash         = &map_fn_hash;
	return mt;
}

bool map_check(struct object *self)
{
	return is_instance(self, vm_get()->map_type);
}

/******************************************************************/

static void map_fn_initialize(object_t *obj)
{
	map_initialize((map_t*)obj);
}

static void map_fn_deinitialize(object_t *obj)
{
	map_deinitialize((map_t*)obj);
}

wchar_t *map_fn_repr(object_t *obj)
{
	return object_fn_repr(obj);
}

size_t map_fn_hash(object_t *self)
{
	return 42; /*XXX*/
}

/******************************************************************/

static const size_t INITIAL_I = 3;
static const size_t PERTURB_SHIFT = 2;

static int foo = 42;
static object_t *dummy = (object_t*)&foo;

struct map_slot {
	size_t hash;
	object_t *key, *value;
};

static void map_insert_h(struct map *map, object_t *key, size_t hash, object_t *value);

static void map_initialize(struct map *map)
{
	map->i = -1;
	map->n = 0;
	map->mask = 0;
	map->slots = NULL;
	map_resize(map, INITIAL_I);
}

/* resize number of slots to i**2 */
static void map_resize(struct map *map, size_t i)
{
	const size_t old_n = map->n; /* old number of slots */
	struct map_slot *old_slots = map->slots;

	/* setup new size */
	map->i = i;
	map->n = (1 << i);
	map->mask = map->n - 1;
	map->slots = (struct map_slot*)malloc(map->n * sizeof(struct map_slot));

	for (size_t j=0; j<map->n; ++j) {
		map->slots[j].hash = 0;
		map->slots[j].key = 0;
		map->slots[j].value = 0;
	}

	/* copy slots over to new */
	for (size_t k=0; k<old_n; ++k) {
		struct map_slot *s = &old_slots[k];
		if (s == 0 || s->key == dummy) continue;
		map_insert_h(map, s->key, s->hash, s->value);
	}
	free(old_slots);
}

static void map_deinitialize(struct map *map)
{
	free(map->slots);
}

static inline int map_compare_keys(object_t *slot_key, object_t *key)
{
	return 0;
}

static struct map_slot *map_find_slot(struct map *map, object_t *key, size_t hash)
{
	struct map_slot *first_free_slot; /* the first free slot for the given 'hash' */
	struct map_slot *slot, *slot0 = &(map->slots[0]);
	size_t mask = map->mask;
	size_t j = hash & mask;
	
	slot = &slot0[j];
	/* identity match? -> found immediately!*/
	if (slot->key == key) return slot;
	/* immediately notice that key is not in map? */
	if (slot->key == 0) return slot;
	/* hash match? equality match? */
	if (slot->key == dummy) {
		first_free_slot = slot;
	} else {
		if (slot->hash == hash) {
			if (map_compare_keys(slot->key, key) == 1) return slot;
		}
		first_free_slot = 0;
	}

	/* ok, key existance in map was not determined immediately. go search the slots... */
	size_t perturb = hash;
	while (true) {
		j = 5*j + perturb + 1;
		slot = &slot0[j & mask];
		if (slot->key == 0) { /* key not in dict. return a free slot (the first free slot if possible) */
			return first_free_slot ? first_free_slot : slot;
		} else if (slot->key == key) { /* found key by identity? */
			return slot;
		} else if (slot->hash == hash && slot->key != dummy) { /* hash match? equality match? */
			if (map_compare_keys(slot->key, key) == 1) return slot;
		} else if (slot->key == dummy && first_free_slot == 0) {
			first_free_slot = slot;
		}
		perturb >>= PERTURB_SHIFT;
	}

	assert(0);
	return 0;
}

/*static Object *map_lookup_h(struct Map *map, Object *key, size_t hash)
{
	struct MapSlot *slot = map_find_slot(map, key, hash);
	if (slot != 0 && slot->key != dummy) {
		return slot->value;
	} else {
		return (Object*)Nil_New();
	}
}*/

static void map_insert_h(struct map *map, object_t *key, size_t hash, object_t *value)
{
	struct map_slot *slot = map_find_slot(map, key, hash);
	if (slot != 0 && slot->key != dummy) {
		/* remove old value */
		/* XXX: DECREF */
	}
	slot->key = key;
	slot->value = value;
	slot->hash = hash;
}
