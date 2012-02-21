#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

typedef struct Parser
{
	struct Scanner *scn;
} Parser;

void parser_init(Parser *p);
void parser_done(Parser *p);

struct ASTNode *parser_run(Parser *p, FILE *f);

#endif
