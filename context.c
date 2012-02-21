#include "context.h"

#include <stdlib.h>

#include "obj/object.h"
#include "obj/instance.h"
#include "obj/class.h"
#include "obj/nil.h"

struct Context *ctx = 0;
	
/* Setup classes: Object, Class, Nil, True, False */
static void create_classes()
{
	/* I. Setup the most basic classes: Object, Class, Nil */
	ctx->object_class = class_construct_object_class(0, 0);
	ctx->class_class  = class_construct_class_class(0, 0);
	ctx->nil_class    = class_construct_nil_class(0, 0);

	ctx->object_class->super.super.clas = ctx->class_class;
	ctx->object_class->base = 0;

	ctx->class_class->super.super.clas = ctx->class_class;
	ctx->class_class->base = ctx->object_class;

	ctx->nil_class->super.super.clas = ctx->class_class;
	ctx->nil_class->base = ctx->object_class;
	
	/* II. Setup basic classes: True, False */
	ctx->true_class  = class_construct_true_class(ctx->class_class, ctx->object_class);
	ctx->false_class = class_construct_false_class(ctx->class_class, ctx->object_class);
}

/* Create canonical objects: nil, true, false */
static void create_objects()
{
	ctx->nil    = nil_construct(ctx->nil_class, sizeof(Instance));
	ctx->true_  = instance_construct(ctx->true_class, sizeof(Instance));
	ctx->false_ = instance_construct(ctx->false_class, sizeof(Instance));
}

/* Add predefined methods to basic classes */
static void create_class_methods()
{
}

void context_init()
{
	ctx = (Context*)malloc(sizeof(Context));

	create_classes();
	create_objects();
	create_class_methods();
}

void context_deinit()
{
	free(ctx);
}
