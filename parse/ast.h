#ifndef AST_H
#define AST_H

#include <stddef.h>

#define AST_INVALID       (-1)

/* constant expressions */
#define AST_NIL              0 /* constant nil expression */
#define AST_INTEGER          1
#define AST_STRING           2
#define AST_IDENTIFIER       3

/* unary operations */
#define AST_NEGATE         100 /* negate expression */

/* binary operations */
#define AST_ADD            200 /* add */
#define AST_SUB            201 /* subtract */
#define AST_MUL            202
#define AST_DIV            203

/* relational operators */
#define AST_IS_EQUAL       300
#define AST_IS_NOT_EQUAL   301
#define AST_IS_LESS        302
#define AST_IS_GREATER     303
#define AST_IS_LESS_EQ     304
#define AST_IS_GREATER_EQ  305

/* assignment */
#define AST_ASSIGN         400
#define AST_MUL_ASSIGN     401 /* '*=' */
#define AST_DIV_ASSIGN     402
#define AST_ADD_ASSIGN     403
#define AST_SUB_ASSIGN     404 

/* special forms */
#define AST_IF             500 /* sub[0]: if-expr, sub[1]: then-expr, sub[2]: else-expr (or NULL) */
#define AST_DEF            501 /* sval: identifier, sub[0]: lambda-expr ??? */
#define AST_METHOD_CALL    502
#define AST_LET            503
#define AST_BODY           504 /* sequence of statemets */

typedef struct ASTNode
{
	int id; /* one of AST_xyz */

	/* generic slots */
	struct ASTNode *sub[3]; /* children (NULL if unused) */
	int ival;
	char *sval;
	double fval;

	union Info {
		struct CallMethodInfo {
			char *method; /* method name */
			struct ASTNode *receiver;
			struct ASTNode **args; int argc;
			struct ASTNode *do_expr; /* either 0 or a do-expression */
		} method_call;

		struct LetInfo {
			char *ident;
			struct ASTNode *init;
			struct ASTNode *body;
		} let;

		struct BodyInfo {
			int num_stmts;
			struct ASTNode **stmts;
		} body;
	} v;
} ASTNode;

ASTNode *ast_create(int id);
ASTNode *ast_create_1(int id, ASTNode *sub0);
ASTNode *ast_create_2(int id, ASTNode *sub0, ASTNode *sub1);
ASTNode *ast_create_3(int id, ASTNode *sub0, ASTNode *sub1, ASTNode *sub2);
void ast_destroy(ASTNode *node); /* recursive! */

void ast_print(ASTNode *ast, int indent); /* debug print */

ASTNode *ast_create_nil(); /* AST_NIL */
ASTNode *ast_create_integer(int i); /* AST_INTEGER */
ASTNode *ast_create_string(const char *str); /* AST_STRING */
ASTNode *ast_create_identifier(const char *str); /* AST_IDENTIFIER */

ASTNode *ast_create_method_call(ASTNode *receiver, const char *method, int argc, ASTNode **args, ASTNode *do_expr);
ASTNode *ast_create_let(const char *ident, ASTNode *init, ASTNode *body);
ASTNode *ast_create_body(int num_stmts, ASTNode **stmts);

#endif
