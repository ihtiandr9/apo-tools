#ifndef H_PARSER_H
#define H_PARSER_H

#include <lexer.h>
#include <nodes.h>

typedef struct _Parser
{
    Node *statement;
    int level;
    void (*parse)(struct _Parser *self, pLexer lexer);
} *pParser, Parser;

// exports
pParser parser_create(void);
void parser_free(pParser parser);
#endif
