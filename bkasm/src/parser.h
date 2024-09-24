#ifndef H_PARSER_H
#define H_PARSER_H

#include <globals.h>
#include <lexer.h>
#include <nodes.h>

typedef union ParseResult ParseResult;
union ParseResult
{
    Node node;
    Expr expr;
};

STRUCT(Parser)
{
    ParseResult *statement;
    int level;
    void (*parse)(Parser *self, Lexer* lexer, Program *prog);
};

// exports
Parser* parser_create(void);
int parser_init(Parser* parser);
void parser_free(Parser* parser);
#endif
