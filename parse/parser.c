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
static ASTNode *parse_comparison(Parser *p);
static ASTNode *parse_relational(Parser *p);
static ASTNode *parse_clause(Parser *p);
static ASTNode *parse_term(Parser *p);
static ASTNode *parse_dot(Parser *p);
static ASTNode *parse_atomic(Parser *p);

/* return current token from scanner */
static Token *peek(Parser *p)
{
	return scanner_peek(p->scn);
}

/* return current token id from scanner */
static int peek_id(Parser *p)
{
	return peek(p)->id;
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

	if (peek_id(p) == tok_id) {
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
	while (peek_id(p) == TOK_END_OF_LINE) {
		accept(p);
	}
}

/* main parser */
static ASTNode *parse_file(Parser *p)
{
	/* parse TOK_BEG_OF_FILE */
	if (peek_id(p) != TOK_BEG_OF_FILE) {
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

	switch (peek_id(p)) {
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
	switch (peek_id(p)) {
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
	switch (peek_id(p)) {
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

/* +, - */
static ASTNode *parse_clause(Parser *p) /* better name? */
{
	ASTNode *expr = parse_term(p);

loop:
	switch (peek_id(p))
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
	ASTNode *expr = parse_dot(p);

loop:
	switch (peek_id(p)) {
		case TOK_ASTERISK:
			accept(p); ignore_eols(p);
			expr = ast_create_2(AST_MUL, expr, parse_dot(p));
			goto loop;

		case TOK_SLASH:
			accept(p); ignore_eols(p);
			expr = ast_create_2(AST_DIV, expr, parse_dot(p));
			goto loop;
	}

	return expr;
}

/*
 * Checks if current token is the first token of an expression.
 */
static int look_ahead_for_expr(Parser *p)
{
	switch (peek_id(p)) {
		case TOK_INTEGER:
		case TOK_STRING:
		case TOK_LPAR:
		case TOK_IDENTIFIER: return 1;
	}
	return 0;
}

/*
   <parameter_list> ::= '(' <expr1> ',' <expr2> ',' ... ',' <exprN> ')'
 */

static ASTNode **parameter_list_parser_inner(Parser *p, int *argc_out /*out*/, int have_surrounding_parentheses)
{
	int argc = 0; ASTNode **args = NULL;

	int have_comma = 0;

	while (have_surrounding_parentheses ? (peek_id(p) != TOK_RPAR) 
	                                    : (have_comma || (argc == 0 && look_ahead_for_expr(p)))) {
		/* we need to have had a comma for all but the first argument */
		if (argc > 0 && !have_comma) {
			error(p, "expected comma to separate arguments in argument list");
		}

		/* parse one argument */
		if (!look_ahead_for_expr(p)) {
			error(p, "expected argument after comma in argument list");
		}

		ASTNode *arg = parse_expr(p); 
		argc += 1;
		args = (ASTNode**)realloc(args, argc * sizeof(ASTNode*));
		args[argc-1] = arg;

		/* parse comma */
		if (peek_id(p) == TOK_COMMA) {
			accept(p);
			have_comma = 1;
		} else {
			have_comma = 0;
		}
	}

	if (have_comma) {
		error(p, "expected argument after comma in argument list");
	}

	*argc_out = argc;
	return args;
}

static ASTNode **parameter_list_parser(Parser *p, int *argc_out /*out*/)
{
	int argc; ASTNode **args;

	if (peek_id(p) == TOK_LPAR) { /* with surrounding parentheses */
		expect(p, TOK_LPAR, "expected opening parenthesis at begining of argument list"); ignore_eols(p);
		args = parameter_list_parser_inner(p, &argc, 1);
		expect(p, TOK_RPAR, "expected closing parenthesis at end of argument list"); ignore_eols(p);
	} else { /* without surrounding parentheses */
		args = parameter_list_parser_inner(p, &argc, 0);
	}

	*argc_out = argc;
	return args;
}

static ASTNode *method_call_parser(Parser *p, ASTNode *receiver)
{
	/* accept the dot '.' token */
	expect(p, TOK_DOT, "expected dot '.' token"); ignore_eols(p);

	/* we can only parse a dot if an identifier follows */
	if (peek_id(p) == TOK_IDENTIFIER) {
		/* method */
		char *method = strdup(peek(p)->sval); accept(p);
		
		/* have call parameters? */
		int argc = 0;
		ASTNode **args = parameter_list_parser(p, &argc);

		/* have do block? */
		ASTNode *do_expr = NULL;

		ASTNode *expr = ast_create_method_call(receiver, method, argc, args, do_expr);
		free(method);
		return expr;
	} else {
		error(p, "need identifier after dot '.' expression");
		return NULL;
	}
}

/* the dot operator: <expr>.<identifier> */
static ASTNode *parse_dot(Parser *p)
{
	ASTNode *expr = parse_atomic(p);

	while (peek_id(p) == TOK_DOT) {
		expr = method_call_parser(p, expr);
	}

	return expr;
}

/* atomic expressions like literals, but also if-then-else, (<expr>*). */
static ASTNode *parse_atomic(Parser *p)
{
	ASTNode *tmp;
	int i;
	char *s;

	switch (peek_id(p)) {
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

		case TOK_IDENTIFIER:
			s = strdup(peek(p)->sval); accept(p);
			tmp = ast_create_identifier(s);
			free(s);
			return tmp;

		default:
			assert(0);
	}
}
