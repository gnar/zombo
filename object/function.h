#ifndef FUNCTION_H
#define FUNCTION_H

#include "object/object.h"
#include "object/type.h"

#include <stdint.h>

typedef struct function
{
	object_t super;

	/* byte code */
	size_t num_instr, instr_size; /* number of used/available entries in 'instr' and 'instr_args' */
	int *instr; /* the instructions */
	int *instr_args;
	size_t stack_size; /* stack size needed to execute code in 'instr' */

	/* constants table */
	size_t num_constants;
	object_t *constants;

	/* number of passed arguments (on the stack) */
	unsigned int argc;

	/* number and names of local lexical variables introduced by this function */
	int num_locals;
	char **locals;
} function_t;

function_t *function_new();
void function_add_instr(function_t *fn, int opcode, int arg);

type_t *functiontype_new();
bool function_check(object_t *self);

void function_debug_print(function_t *fn);

/* instructions */

#define I_NOP          0    /* NOP */

#define I_PUSHI       10    /* PUSHI <int>         -- push integer */
#define I_PUSHC       11    /* PUSHC <int>         -- push constant */
#define I_PUSHL       12    /* PUSHL <int>         -- push lexical variable */

#define I_RET         40    /* RET */

#define I_POP         20    /* POP                 -- pop stack top */
#define I_POPL        21    /* POPL <int>          -- pop and save to lexical variable */

#define I_NEG         30    /* unary */
#define I_ADD         31    /* binary (4x) */
#define I_SUB         32   
#define I_MUL         33   
#define I_DIV         34   

#define I_APPLY       38    /* APPLY <int>         -- call a callable with <int> arguments */

#define I_PRINT       80    /* PRINT               -- print and pop stack top element */

#endif
