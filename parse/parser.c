#include "parser.h"
#include "scanner.h"
#include "ast.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

void parser_init(Parser *p)
{
	p->scn = NULL;
}

void parser_done(Parser *p)
{
}

static ASTNode *parse_file(Parser *p);

ASTNode *parser_run(Parser *p, FILE *f)
{
	ASTNode *root = NULL;

	/* create scanner on f */
	p->scn = (Scanner*)malloc(sizeof(Scanner));
	scanner_init(p->scn, f);

	/* parse a file */
	root = parse_file(p);

	/* cleanup scanner */
	scanner_done(p->scn);
	free(p->scn);
	p->scn = NULL;

	/* done */
	return root;
}

/***********************************************************/

static ASTNode *parse_expr(Parser *p);
static ASTNode *parse_assign(Parser *p);
static ASTNode *parse_clause(Parser *p);
static ASTNode *parse_term(Parser *p);
static ASTNode *parse_atomic(Parser *p);
static ASTNode *parse_relational(Parser *p);
static ASTNode *parse_comparison(Parser *p);

/* return current token from scanner */
static Token *peek(Parser *p)
{
	return scanner_peek(p->scn);
}

/* accept current token */
static void accept(Parser *p)
{
	scanner_accept(p->scn);
}

/* flag error */
static void error(Parser *p, const char *fmt, ...)
{
	va_list args;
	char buf[1024];

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	printf("Parse error: %s\n", buf);
	exit(-1);
}

/* expect token, else flag error */
static void expect(Parser *p, int tok_id, const char *fmt, ...)
{
	va_list args;

	if (peek(p)->id == tok_id) {
		accept(p);
	} else {
		/* parse error! */
		va_start(args, fmt);
		error(p, fmt, args);
		va_end(args);
	}
}

/* ignore zero to N end-of-line tokens */
static void ignore_eols(Parser *p)
{
	while (peek(p)->id == TOK_END_OF_LINE) {
		accept(p);
	}
}

/* main parser */
static ASTNode *parse_file(Parser *p)
{
	/* parse TOK_BEG_OF_FILE */
	if (peek(p)->id != TOK_BEG_OF_FILE) {
		assert(0);
	}
	accept(p);

	return parse_expr(p);
}


static ASTNode *parse_expr(Parser *p)
{
	return parse_assign(p);
}

/* Operators: =, +=, -=, /=, *= (right-associative) */
static ASTNode *parse_assign(Parser *p)
{
	ASTNode *expr = parse_comparison(p);

	switch (peek(p)->id) {
		case TOK_ASSIGN:
			accept(p); ignore_eols(p);
			expr = ast_create_2(AST_ASSIGN, expr, parse_assign(p));
			break;
	}

	return expr;
}

/* Operators: ==, != (left-associative) */
static ASTNode *parse_comparison(Parser *p) 
{
	ASTNode *expr = parse_relational(p);

loop:
	switch (peek(p)->id) {
		case TOK_EQUAL: /* == */
			accept(p); ignore_eols(p);
			expr = ast_create_2(AST_IS_EQUAL, expr, parse_comparison(p));
			goto loop;

		case TOK_NOT_EQUAL: /* != */
			accept(p); ignore_eols(p);
			expr = ast_create_2(AST_IS_NOT_EQUAL, expr, parse_comparison(p));
			goto loop;
	}

	return expr;
}

/* Operators: <, >, <=, >= (left-associative) */
static ASTNode *parse_relational(Parser *p)
{
	ASTNode *expr = parse_clause(p);

loop:
	switch (peek(p)->id) {
		case TOK_LESS: /* < */
			accept(p); ignore_eols(p);
			expr = ast_create_2(AST_IS_LESS, expr, parse_clause(p));
			goto loop;
		case TOK_GREATER:
			accept(p); ignore_eols(p);
			expr = ast_create_2(AST_IS_GREATER, expr, parse_clause(p));
			goto loop;
		case TOK_LESS_EQUAL:
			accept(p); ignore_eols(p);
			expr = ast_create_2(AST_IS_LESS_EQ, expr, parse_clause(p));
			goto loop;
		case TOK_GREATER_EQUAL:
			accept(p); ignore_eols(p);
			expr = ast_create_2(AST_IS_GREATER_EQ, expr, parse_clause(p));
			goto loop;
	}

	return expr;
}

/* the dot operator: <expr>.<identifier> */
/*static ASTNode *parse_dot(Parser *p)
{
	return parse_clause(p);
}*/

/* +, - */
static ASTNode *parse_clause(Parser *p) /* better name? */
{
	ASTNode *expr = parse_term(p);

loop:
	switch (peek(p)->id)
	{
		case TOK_PLUS:
			accept(p); ignore_eols(p);
			expr = ast_create_2(AST_ADD, expr, parse_term(p));
			goto loop;

		case TOK_MINUS:
			accept(p); ignore_eols(p);
			expr = ast_create_2(AST_SUB, expr, parse_term(p));
			goto loop;
	}

	return expr;
}

/* *, /, %, etc. */
static ASTNode *parse_term(Parser *p)
{
	ASTNode *expr = parse_atomic(p);

loop:
	switch (peek(p)->id) {
		case TOK_ASTERISK:
			accept(p); ignore_eols(p);
			expr = ast_create_2(AST_MUL, expr, parse_atomic(p));
			goto loop;

		case TOK_SLASH:
			accept(p); ignore_eols(p);
			expr = ast_create_2(AST_DIV, expr, parse_atomic(p));
			goto loop;
	}

	return expr;
}

/* atomic expressions like literals, but also if-then-else, (<expr>*). */
static ASTNode *parse_atomic(Parser *p)
{
	ASTNode *tmp;
	int i;
	char *s;

	switch (peek(p)->id) {
		case TOK_INTEGER:
			i = peek(p)->ival; accept(p);
			return ast_create_integer(i);

		case TOK_STRING:
			s = strdup(peek(p)->sval); accept(p);
			tmp = ast_create_string(s);
			free(s);
			return tmp;

		case TOK_LPAR: /* term = '(' expr ')' */
			accept(p); ignore_eols(p);
			tmp = parse_expr(p); ignore_eols(p);
			expect(p, TOK_RPAR, "expected closing parenthesis");
			return tmp;

		default:
			assert(0);
	}
}
