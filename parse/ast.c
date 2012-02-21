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

	/* special cleanup code */
	switch (node->id) {
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

void ast_print(ASTNode *ast, int indent)
{
	int i;

	/* indenting */
	for (i=0; i<indent; ++i) {
		printf("    ");
	}

	/* print ast recursively */
	switch (ast->id) {
		case AST_NIL:           printf("nil"); break;
		case AST_INTEGER:       printf("integer:%i", ast->ival); break;
		case AST_STRING:        printf("string:%s", ast->sval); break;
		case AST_IDENTIFIER:    printf("ident:%s", ast->sval); break;

		case AST_NEGATE:        printf("negate"); break;

		case AST_ADD:           printf("add"); break;
		case AST_SUB:           printf("sub"); break;
		case AST_MUL:           printf("mul"); break;
		case AST_DIV:           printf("div"); break;

		case AST_ASSIGN:        printf("assign"); break;

		case AST_IS_EQUAL:      printf("== equal"); break;
		case AST_IS_NOT_EQUAL:  printf("!= not_equal"); break;
		case AST_IS_LESS:       printf("< less"); break;
		case AST_IS_GREATER:    printf("> greater"); break;
		case AST_IS_LESS_EQ:    printf("<= less_equal"); break;
		case AST_IS_GREATER_EQ: printf(">= greater_equal"); break;

		case AST_IF:            printf("if"); break;
		case AST_DEF: assert(0);
		case AST_CALL_METHOD:   printf("call: msg='%s', argc=%i", ast->sval, ast->ival); break;
	}
	printf("\n");

	/* recurse */
	for (i=0; i<3; ++i) {
		if (ast->sub[i]) ast_print(ast->sub[i], indent+1);
	}
}
