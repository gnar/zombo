#include "ast.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

ASTNode *ast_create(int id)
{
	size_t i;
	ASTNode *node = (ASTNode*)malloc(sizeof(*node));
	node->id = id;
	node->ival = 0;
	node->sval = NULL;
	node->fval = 0.0f;
	for (i=0; i<3; ++i) node->sub[i] = NULL;
	return node;
}

ASTNode *ast_create_1(int id, ASTNode *sub0)
{
	ASTNode *node = ast_create(id);
	node->sub[0] = sub0;
	return node;
}

ASTNode *ast_create_2(int id, ASTNode *sub0, ASTNode *sub1)
{
	ASTNode *node = ast_create(id);
	node->sub[0] = sub0;
	node->sub[1] = sub1;
	return node;
}

ASTNode *ast_create_3(int id, ASTNode *sub0, ASTNode *sub1, ASTNode *sub2)
{
	ASTNode *node = ast_create(id);
	node->sub[0] = sub0;
	node->sub[1] = sub1;
	node->sub[2] = sub2;
	return node;
}

void ast_destroy(ASTNode *node)
{
	size_t i;

	if (node == NULL) return;

	/* special cleanup code */
	switch (node->id) {
		case AST_METHOD_CALL: {
			ast_destroy(node->v.method_call.receiver);
			ast_destroy(node->v.method_call.do_expr);
			for (i=0; i<node->v.method_call.argc; ++i) {
				ast_destroy(node->v.method_call.args[i]);
			}
			free(node->v.method_call.args);
			free(node->v.method_call.method);
			break;
		}

		case AST_BODY: {
			for (i=0; i<node->v.body.num_stmts; ++i) {
				ast_destroy(node->v.body.stmts[i]);
			}
			free(node->v.body.stmts);
			break;
		}
	}

	/* delete string info if any */
	free(node->sval); node->sval = NULL;

	/* recursively destropy children */
	for (i=0; i<3; ++i) {
		if (node->sub[i]) {
			ast_destroy(node->sub[i]);
		}
	}

	/* dealloc */
	free(node);
}

/* constant expressions */
ASTNode *ast_create_nil()
{
	return ast_create(AST_NIL);
}

ASTNode *ast_create_integer(int i)
{
	ASTNode *node = ast_create(AST_INTEGER);
	node->ival = i;
	return node;
}

ASTNode *ast_create_string(const char *str)
{
	ASTNode *node = ast_create(AST_STRING);
	node->sval = strdup(str);
	return node;
}

ASTNode *ast_create_identifier(const char *str)
{
	ASTNode *node = ast_create(AST_IDENTIFIER);
	node->sval = strdup(str);
	return node;
}

ASTNode *ast_create_method_call(ASTNode *receiver, const char *method, int argc, ASTNode **args, ASTNode *do_expr)
{
	ASTNode *node = ast_create(AST_METHOD_CALL);
	node->v.method_call.receiver = receiver;
	node->v.method_call.method = strdup(method);
	node->v.method_call.argc = argc;
	node->v.method_call.args = args;
	node->v.method_call.do_expr = do_expr;
	return node;
}

ASTNode *ast_create_let(const char *ident, ASTNode *init, ASTNode *body)
{
	ASTNode *node = ast_create(AST_LET);
	node->v.let.ident = strdup(ident);
	node->v.let.init = init;
	node->v.let.body = body;
	return node;
}

ASTNode *ast_create_body(int num_stmts, ASTNode **stmts)
{
	ASTNode *node = ast_create(AST_BODY);
	node->v.body.num_stmts = num_stmts;
	node->v.body.stmts = stmts;
	return node;
}

void ast_print(ASTNode *ast, int indent)
{
	int i;

	/* indenting */
	char space[1024] = {' '};
	for (i=0; i<indent*4; ++i) space[i] = ' ';
	space[indent*4] = '\0';

	/* print ast recursively */
	switch (ast->id) {
		case AST_NIL:           printf("%snil\n", space); break;
		case AST_INTEGER:       printf("%sinteger:%i\n", space, ast->ival); break;
		case AST_STRING:        printf("%sstring:%s\n", space, ast->sval); break;
		case AST_IDENTIFIER:    printf("%sident:%s\n", space, ast->sval); break;

		case AST_NEGATE:        printf("%snegate\n", space); break;

		case AST_ADD:           printf("%sadd\n", space); break;
		case AST_SUB:           printf("%ssub\n", space); break;
		case AST_MUL:           printf("%smul\n", space); break;
		case AST_DIV:           printf("%sdiv\n", space); break;

		case AST_ASSIGN:        printf("%sassign\n", space); break;

		case AST_IS_EQUAL:      printf("%s== equal\n", space); break;
		case AST_IS_NOT_EQUAL:  printf("%s!= not_equal\n", space); break;
		case AST_IS_LESS:       printf("%s< less\n", space); break;
		case AST_IS_GREATER:    printf("%s> greater\n", space); break;
		case AST_IS_LESS_EQ:    printf("%s<= less_equal\n", space); break;
		case AST_IS_GREATER_EQ: printf("%s>= greater_equal\n", space); break;

		case AST_IF:            printf("%sif\n", space); break;
		case AST_DEF: assert(0);

		case AST_METHOD_CALL:   {
			printf("%scall: msg='%s', argc=%i\n", space, ast->v.method_call.method, ast->v.method_call.argc);

			printf("%s    recv:\n", space);
			ast_print(ast->v.method_call.receiver, indent+2);

			for (i=0; i<ast->v.method_call.argc; ++i) {
				printf("%s    arg_%i:\n", space, i);
				ast_print(ast->v.method_call.args[i], indent+2);
			}

			if (ast->v.method_call.do_expr) {
				printf("%s    do::\n", space);
				ast_print(ast->v.method_call.do_expr, indent+2);
			}
			break;
		}

		case AST_BODY: {
			printf("%sbody: num_stmts=%i\n", space, ast->v.body.num_stmts);

			for (i=0; i<ast->v.body.num_stmts; ++i) {
				printf("%s    stmt_%i:\n", space, i);
				ast_print(ast->v.body.stmts[i], indent+2);
			}
			break;
		}
	}

	/* recurse */
	for (i=0; i<3; ++i) {
		if (ast->sub[i]) ast_print(ast->sub[i], indent+1);
	}
}
