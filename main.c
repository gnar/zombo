#include <stdio.h>
#include <stdlib.h>

#include "context.h"

#include "parse/scanner.h"
#include "parse/parser.h"
#include "parse/ast.h"

#include "obj/object.h"
#include "obj/instance.h"
#include "obj/class.h"

void test0()
{
	char buf[256];

	FILE *f = fopen("test.g4", "r");

	printf("Tokens:\n");

	Scanner scn;
	scanner_init(&scn, f);
	do {
		token_print(scanner_peek(&scn), buf);
		printf("    %s\n", buf);

		scanner_accept(&scn);
	} while ((scanner_peek(&scn)->id != TOK_END_OF_FILE) && (scanner_peek(&scn)->id != TOK_ERROR));

	token_print(scanner_peek(&scn), buf);
	printf("    %s\n", buf);

	scanner_done(&scn);

	fclose(f);
}

void test1()
{
	Parser p;
	parser_init(&p);

	FILE *f = fopen("test.g4", "r");
	ASTNode *root = parser_run(&p, f);
	fclose(f);

	parser_done(&p);

	/* print ast */
	printf("Syntax tree:\n");
	ast_print(root, 1);
	ast_destroy(root);
}

void test2()
{
	ASTNode *i0 = ast_create_integer(45);
	ASTNode *i1 = ast_create_string("hallo");
	ASTNode *ad = ast_create_2(AST_ADD, i0, i1);

	ast_print(ad, 0);

	ast_destroy(ad);
}


int main()
{
	test0();
	test1();

	/*context_init();
 
	char *tmp = object_internal_repr((Object*)ctx->nil);
	printf("%s\n", tmp);
	printf("%p\n", ctx->nil);
	free(tmp);

	context_deinit();*/

	return 0;
}
