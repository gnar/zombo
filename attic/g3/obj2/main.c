#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "instance.h"
#include "class.h"

Class *object_class;
Class *nil_class;
Class *class_class;
Class *true_class;
Class *false_class;

void construct_classes()
{
	// Setup classes: Object, Class, Nil, True, False.

	// I. Setup the most basic classes: Object, Class, Nil.
	object_class = class_construct_object_class(0, 0);
	class_class  = class_construct_class_class(0, 0);
	nil_class    = class_construct_nil_class(0, 0);

	object_class->super.super.clas = class_class;
	object_class->base = 0;

	class_class->super.super.clas = class_class;
	class_class->base = object_class;

	nil_class->super.super.clas = class_class;
	nil_class->base = object_class;
	
	// II. Setup basic classes: True, False
	true_class  = class_construct_true_class(class_class, object_class);
	false_class = class_construct_false_class(class_class, object_class);

	// III. Add predefined methods to basic classes.
};


int main()
{
	construct_classes();

	Object *nil = class_construct_object(nil_class);

	char *tmp = object_internal_repr(nil);
	printf("%s\n", tmp);
	free(tmp);

	free(nil);
}
