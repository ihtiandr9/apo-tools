#ifndef H_PARSER_H
#define H_PARSER_H

#include <bkasm.h>
#include <lexer.h>
#include <nodes.h>
#include <asmast.h>

STRUCT(Parser)
{
    ASTree *prog;
    int level;
};

// exports
Parser* parser_create(void);
int parser_init(Parser* parser);
void parser_free(Parser* parser);
void parser_parse(Parser *self, Lexer *lexer);
#endif
