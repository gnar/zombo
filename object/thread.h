#ifndef THREAD_H
#define THREAD_H

#include "object/object.h"
#include "object/closure.h"

typedef struct frame
{
	object_t super;

	/* the associated closure */
	struct closure *clos; 
	
} frame_t;

typedef struct thread
{
	object_t super;
	struct frame *frm; /* the currently executed frame */
} thread_t;

struct type *threadtype_new();

/* create a new thread that executes a closure with zero arguments */
thread_t *thread_new(closure_t *clos);

/* execute a thread */
void thread_execute(thread_t *thr);

#endif
