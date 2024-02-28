#ifndef H_PARSER_H
#define H_PARSER_H

#include <lexer.h>
#include <nodes.h>

typedef struct _Parser
{

    Node *statement;

    void (*parse)(struct _Parser *self, pLexer lexer);

} Parser, *pParser;

pParser parser_create(void);
void parser_free(pParser parser);

#endif
