#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>

/* List of token types */

/* error */
#define TOK_ERROR               -1

/* literals */
#define TOK_IDENTIFIER 		256
#define TOK_STRING 		258
#define TOK_INTEGER 		259
#define TOK_FLOAT 		260

/* keywords */
#define TOK_DO			301
#define TOK_BEGIN 		302
#define TOK_END 		303
#define TOK_IF 			304
#define TOK_THEN		305
#define TOK_ELSE		306
#define TOK_DEF 		307
#define TOK_LET 		308

/* special */
#define TOK_END_OF_LINE 	400
#define TOK_END_OF_FILE 	401
#define TOK_BEG_OF_LINE 	403
#define TOK_BEG_OF_FILE 	404

/* punctation */
#define TOK_LPAR 		500 /* ( */
#define TOK_RPAR 		501 /* ) */
#define TOK_COLON 		502 /* : */
#define TOK_LQUOTE 		503 /* << */
#define TOK_RQUOTE 		504 /* >> */
#define TOK_COMMA 		505 /* : */
#define TOK_PIPE 		506 /* | */
#define TOK_ASSIGN 		507 /* = */
#define TOK_PLUS 		508 /* + */
#define TOK_MINUS 		509 /* - */
#define TOK_ASTERISK 		510 /* * */
#define TOK_SLASH 		511 /* "/" */
#define TOK_EQUAL 		512 /* == */
#define TOK_NOT_EQUAL 		513 /* != */
#define TOK_LESS 		514 /* < */
#define TOK_GREATER 		515 /* > */
#define TOK_LESS_EQUAL 		516 /* <= */
#define TOK_GREATER_EQUAL 	517 /* >= */
#define TOK_DOT 		518 /* . */
#define TOK_SHARP 		519 /* # */
#define TOK_SQLPAR 		520 /* [ */
#define TOK_SQRPAR 		521 /* ] */
#define TOK_AMPERSAND 		522 /* & */

typedef struct Token
{
	int id; /* one of TOK_xyz */
	int line;
	int ival;
	double fval;
	char *sval;
} Token;

void token_print(struct Token *tok, char *buf);

typedef struct Scanner
{
	FILE *f;
	int line;
	int ch; /* next character to be scanned */
	struct Token tok; /* current token */
	int error; /* true if parse error occured (then scn->tok is an TOK_ERROR token) */
} Scanner;

void scanner_init(Scanner *scn, FILE *f);
void scanner_done(Scanner *scn);

Token *scanner_peek(Scanner *scn);
void scanner_accept(Scanner *scn);

#endif
