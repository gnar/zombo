#ifndef FUNCTION_H
#define FUNCTION_H

#include "object/object.h"
#include "object/type.h"

#include <stdint.h>

typedef struct function
{
	object_t super;

	/* byte code */
	size_t num_instr;
	unsigned char *instr; /* the instructions */
	unsigned int *instr_args;
	size_t stack_size; /* stack size needed to execute code in 'instr' */

	/* constants table */
	size_t num_constants;
	object_t *constants;

	/* number and names of local lexical variables introduced by this function */
	int num_locals;
	char **locals;
} function_t;

function_t *function_new();

type_t *functiontype_new();
bool function_check(object_t *self);

/* instructions */

#define I_NOP          0    /* NOP */

#define I_PUSHI       10    /* PUSHI <int>         -- push integer */
#define I_PUSHC       11    /* PUSHC <int>         -- push constant */
#define I_PUSHL       11    /* PUSHL <int>         -- push lexical variable */
#define I_PUSHR       12    /* PUSHR               -- push return-continuation of current frame */

#define I_POP         20    /* POP                 -- pop stack top */
#define I_POPL        21    /* POPL <int>          -- pop and save to lexical variable */

#define I_UNOP        30    /* UNOP <int>          -- unary operator call */
#define I_BINOP       31    /* BINOP <int>         -- binary operator call */
#define I_APPLY       32    /* APPLY <int>         -- call a callable with <int> arguments */

#define I_PRINT       40    /* PRINT               -- print and pop stack top element */

#define I_UNOP_PLUS   0  /* +a */
#define I_UNOP_MINUS  1  /* -a */

#define I_BINOP_ADD   0  /* a+b */
#define I_BINOP_SUB   1  /* a-b */
#define I_BINOP_MUL   2  /* a*b */
#define I_BINOP_DIV   3  /* a/b */
#define I_BINOP_POW   4  /* a**b */

#endif
