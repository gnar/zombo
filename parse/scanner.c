#include "scanner.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

static void scanner_scan(Scanner *scn);
static void scanner_scan_integer(Scanner *scn);
static void scanner_scan_identifier_or_keyword(Scanner *scn);
static void scanner_scan_string(Scanner *scn);
static void scanner_next_char(Scanner *scn);

/* special values for Scanner::ch */
#define BEG_OF_FILE 260
#define END_OF_LINE 256
#define END_OF_FILE 257

/* map of keywords */
struct Keyword {
	const char *keyword; /* keyword */
	int id; /* a TOK_xyz constant */
};

static struct Keyword kw_map[] = {
	{"do",    TOK_DO},
	{"begin", TOK_BEGIN},
	{"end",   TOK_END},
	{"if",    TOK_IF},
	{"then",  TOK_THEN},
	{"else",  TOK_ELSE},
	{"def",   TOK_DEF},
	{"let",   TOK_LET},
};

void scanner_init(Scanner *scn, FILE *f)
{
	scn->f = f;
	scn->line = 1;
	scn->ch = BEG_OF_FILE;
	scn->error = 0;

	scn->tok.id = TOK_ERROR;
	scn->tok.ival = 0;
	scn->tok.fval = 0.0;
	scn->tok.sval = 0;

	scanner_accept(scn);
}

Token *scanner_peek(Scanner *scn)
{
	return &(scn->tok);
}

void scanner_accept(Scanner *scn)
{
	if (scn->error) {
		return; /* keep current TOK_ERROR token in scn->tok */
	}

	/* Clear up accepted token */
	free(scn->tok.sval);
	scn->tok.id = TOK_ERROR;
	scn->tok.ival = 0;
	scn->tok.fval = 0.0;
	scn->tok.sval = 0;

	/* Fetch next token */
	scanner_scan(scn);
}

void scanner_done(Scanner *scn)
{
	free(scn->tok.sval);
	scn->f = 0;
}

static void scanner_next_char(Scanner *scn)
{
	char ch;
	size_t br;

	br = fread(&ch, sizeof(char), 1, scn->f);
	if (br == 1) {
		scn->ch = ch;
	} else {
		scn->ch = -1;
	}

	/* Special cases */
	if (feof(scn->f)) {
		scn->ch = END_OF_FILE;
	} else if (scn->ch == '\n') {
		scn->ch = END_OF_LINE;
		scn->line += 1;
	}
}

static void scanner_scan(Scanner *scn)
{
	/* skip whitespace */
	while (scn->ch == ' ' || scn->ch == '\t') {
		scanner_next_char(scn);
	}

	Token *tok = &(scn->tok);
	tok->line = scn->line;

	switch (scn->ch) {
		case BEG_OF_FILE: scanner_next_char(scn); tok->id = TOK_BEG_OF_FILE; break;
		case END_OF_LINE: scanner_next_char(scn); tok->id = TOK_END_OF_LINE; break;
		case END_OF_FILE: scanner_next_char(scn); tok->id = TOK_END_OF_FILE; break;

		case '.': scanner_next_char(scn); tok->id = TOK_DOT; break;
		case '(': scanner_next_char(scn); tok->id = TOK_LPAR; break;
		case ')': scanner_next_char(scn); tok->id = TOK_RPAR; break;
		case ':': scanner_next_char(scn); tok->id = TOK_COLON; break;
		case ',': scanner_next_char(scn); tok->id = TOK_COMMA; break;
		case '|': scanner_next_char(scn); tok->id = TOK_PIPE; break;
		case '+': scanner_next_char(scn); tok->id = TOK_PLUS; break;
		case '-': scanner_next_char(scn); tok->id = TOK_MINUS; break;
		case '*': scanner_next_char(scn); tok->id = TOK_ASTERISK; break;
		case '/': scanner_next_char(scn); tok->id = TOK_SLASH; break;
		case '#': scanner_next_char(scn); tok->id = TOK_SHARP; break;
		case '[': scanner_next_char(scn); tok->id = TOK_SQLPAR; break;
		case ']': scanner_next_char(scn); tok->id = TOK_SQRPAR; break;
		case '&': scanner_next_char(scn); tok->id = TOK_AMPERSAND; break;

		case '=': 
			scanner_next_char(scn); 
			if (scn->ch == '=') {
				scanner_next_char(scn);
				tok->id = TOK_EQUAL; /* == */
			} else {
				tok->id = TOK_ASSIGN; /* = */
			}
			break;

		case '!':
			scanner_next_char(scn);
			if (scn->ch == '=') { /* != */
				scanner_next_char(scn);
				tok->id = TOK_NOT_EQUAL;
			} else { /* ! */
				assert(0);
			}
			break;

		case '<': 
			scanner_next_char(scn);
			if (scn->ch == '<') { /* << */
				scanner_next_char(scn);
				tok->id = TOK_LQUOTE;
			} else if (scn->ch == '=') { /* <= */
				scanner_next_char(scn);
				tok->id = TOK_LESS_EQUAL;
			} else { /* < */
				tok->id = TOK_LESS;
			}
			break;

		case '>': 
			scanner_next_char(scn);
			if (scn->ch == '>') { /* >> */
				scanner_next_char(scn);
				tok->id = TOK_RQUOTE;
			} else if (scn->ch == '=') { /* >= */
				scanner_next_char(scn);
				tok->id = TOK_GREATER_EQUAL;
			} else {
				tok->id = TOK_GREATER; /* < */
			}
			break;

		case '"': 
			scanner_scan_string(scn);
			break;

		default:
			if (isdigit(scn->ch)) {
				scanner_scan_integer(scn);
			} else if (isalpha(scn->ch) || scn->ch == '_') {
				scanner_scan_identifier_or_keyword(scn);
			} else {
				char err_msg[128];
				sprintf(err_msg, "unexpected character '%c'", (char)scn->ch);
				tok->id = TOK_ERROR;
				tok->sval = strdup(err_msg);
				scn->error = 0;
			} 
			break;
	}
}

static void scanner_scan_integer(Scanner *scn)
{
	int is_float = 0;

	int numstr_len = 32; char *numstr = (char*)malloc(numstr_len); 
	int pos = 0;

	while (isdigit(scn->ch)) {
		numstr[pos++] = (char)scn->ch;
		if (pos == numstr_len-1) numstr = (char*)realloc(numstr, numstr_len *= 2);
		scanner_next_char(scn);
	}
	numstr[pos++] = 0;
	
	if (is_float) {
		scn->tok.id = TOK_FLOAT;
		scn->tok.fval = strtod(numstr, 0);
	} else {
		scn->tok.id = TOK_INTEGER;
		scn->tok.ival = atoi(numstr);
	}

	free(numstr);
}

static void scanner_scan_identifier_or_keyword(Scanner *scn)
{
	size_t symstr_len = 64; char *symstr = (char*)malloc(symstr_len);
	int pos = 0;
	size_t i;

	if (isalnum(scn->ch) || scn->ch == '_') {
		symstr[pos++] = (char)scn->ch;
		if (pos == symstr_len-1) symstr = (char*)realloc(symstr, symstr_len *= 2);
		scanner_next_char(scn);
	}

	while (isalnum(scn->ch) || scn->ch == '_' || scn->ch == '!' || scn->ch == '?') {
	
	symstr[pos++] = (char)scn->ch;
		if (pos == symstr_len-1) symstr = (char*)realloc(symstr, symstr_len *= 2);
		scanner_next_char(scn);
	}

	symstr[pos++] = 0;

	/* check if buffer is a keyword */
	for (i=0; i<sizeof(kw_map) / sizeof(kw_map[0]); ++i) {
		if (0 == strcmp(symstr, kw_map[i].keyword)) {
			scn->tok.id = kw_map[i].id;
			free(symstr);
			return;
		}
	}

	/* ok, buffer is an identifier */
	scn->tok.id = TOK_IDENTIFIER;
	scn->tok.sval = symstr;
}

static void scanner_scan_string(Scanner *scn)
{
	size_t str_len = 64; char *str = (char*)malloc(str_len);
	int pos = 0;
	const char *err_msg = 0;

	scanner_next_char(scn); /* skip the '"' */

	while (scn->ch != '"') {
		switch (scn->ch) {
			case END_OF_LINE: err_msg = "Newline inside string"; goto error;
			case END_OF_FILE: err_msg = "End of file inside string"; goto error;

			case '\\': /* escape character */
				scanner_next_char(scn);
				switch (scn->ch) {
					case '\n': str[pos++] = '\n'; break;
					case '\\': str[pos++] = '\\'; break;
					case '"':  str[pos++] = '"'; break;
					case 't':  str[pos++] = '\t'; break;
					case 'a':  str[pos++] = '\a'; break;
					case 'b':  str[pos++] = '\b'; break;
					case 'n':  str[pos++] = '\n'; break;
					case 'r':  str[pos++] = '\r'; break;
					case 'v':  str[pos++] = '\v'; break;
					case 'f':  str[pos++] = '\f'; break;
					case END_OF_LINE: err_msg = "Newline inside string"; goto error;
					case END_OF_FILE: err_msg = "End of file inside string"; goto error;
					default: err_msg = "Unrecognized escape character"; goto error;
				}
				break;

			default: str[pos++] = (char)scn->ch; break;
		}
		
		if (pos == str_len-1) str = (char*)realloc(str, str_len *= 2);

		scanner_next_char(scn);
	}
	scanner_next_char(scn); /* skip ending " */
	str[pos++] = 0;

	scn->tok.id = TOK_STRING;
	scn->tok.sval = str;
	return;

error:
	scn->tok.id = TOK_ERROR;
	scn->tok.sval = strdup(err_msg);
	scn->error = 1;
	free(str);
}

void token_print(struct Token *tok, char *buf)
{
	switch (tok->id) {
		case TOK_ERROR:           sprintf(buf, "error:%s", tok->sval); break;
		case TOK_IDENTIFIER:      sprintf(buf, "identifier:%s", tok->sval); break;
		case TOK_STRING:          sprintf(buf, "string:%s", tok->sval); break;
		case TOK_INTEGER:         sprintf(buf, "integer:%i", tok->ival); break;
		case TOK_FLOAT:           sprintf(buf, "float:%lf", tok->fval); break;
		case TOK_DO:              sprintf(buf, "do"); break;
		case TOK_BEGIN:           sprintf(buf, "begin"); break;
		case TOK_END:             sprintf(buf, "end"); break;
		case TOK_IF:              sprintf(buf, "if"); break;
		case TOK_THEN:            sprintf(buf, "then"); break;
		case TOK_ELSE:            sprintf(buf, "else"); break;
		case TOK_DEF:             sprintf(buf, "def"); break;
		case TOK_LET:             sprintf(buf, "let"); break;
		case TOK_BEG_OF_FILE:     sprintf(buf, "bof"); break;
		case TOK_BEG_OF_LINE:     sprintf(buf, "bol"); break;
		case TOK_END_OF_LINE:     sprintf(buf, "eol"); break;
		case TOK_END_OF_FILE:     sprintf(buf, "eof"); break;
		case TOK_LPAR:            sprintf(buf, "("); break;
		case TOK_RPAR:            sprintf(buf, ")"); break;
		case TOK_COLON:           sprintf(buf, ":"); break;
		case TOK_COMMA:           sprintf(buf, ","); break;
		case TOK_PIPE:            sprintf(buf, "|"); break;
		case TOK_ASSIGN:          sprintf(buf, "="); break;
		case TOK_LQUOTE:          sprintf(buf, "<<"); break;
		case TOK_RQUOTE:          sprintf(buf, ">>"); break;
		case TOK_PLUS:            sprintf(buf, "+"); break;
		case TOK_MINUS:           sprintf(buf, "-"); break;
		case TOK_ASTERISK:        sprintf(buf, "*"); break;
		case TOK_SLASH:           sprintf(buf, "/"); break;
		case TOK_EQUAL:           sprintf(buf, "=="); break;
		case TOK_NOT_EQUAL:       sprintf(buf, "!="); break;
		case TOK_LESS:            sprintf(buf, "<"); break;
		case TOK_GREATER:         sprintf(buf, ">"); break;
		case TOK_LESS_EQUAL:      sprintf(buf, "<="); break;
		case TOK_GREATER_EQUAL:   sprintf(buf, ">="); break;
		case TOK_DOT:             sprintf(buf, "."); break;
		case TOK_SHARP:           sprintf(buf, "#"); break;
		case TOK_SQLPAR:          sprintf(buf, "["); break;
		case TOK_SQRPAR:          sprintf(buf, "]"); break;
		case TOK_AMPERSAND:       sprintf(buf, "&"); break;
		default: assert(0);
	}
}
