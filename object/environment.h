#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "object/object.h"

typedef struct environment 
{
	object_t super;

	struct environment *up;
} environment_t;

#endif
