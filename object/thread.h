#ifndef THREAD_H
#define THREAD_H

#include "object/object.h"

typedef struct thread
{
	object_t super;

} thread_t;

struct type *threadtype_new();

#endif
