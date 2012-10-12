#ifndef CONTINUATION_H
#define CONTINUATION_H

#include "object/object.h"

typedef struct continuation
{
	object_t super;
	
	/* the target frame */
	struct frame *frame;
} continuation_t;

#endif
