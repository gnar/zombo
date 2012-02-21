#ifndef CONTEXT_H
#define CONTEXT_H

typedef struct Context
{
	struct Class *nil_class;
	struct Class *object_class;
	struct Class *class_class;
	struct Class *true_class;
	struct Class *false_class;

	struct Nil *nil;
	struct Instance *true_;
	struct Instance *false_;
} Context;

void context_init();
void context_deinit();

extern Context *ctx;

#endif
