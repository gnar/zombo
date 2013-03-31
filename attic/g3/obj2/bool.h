#ifndef BOOL_TRUE_FALSE_H
#define BOOL_TRUE_FALSE_H

extern Object_vtab true_vtab;

Object *true_construct();

extern Object_vtab false_vtab;

Object *false_construct();

#endif
