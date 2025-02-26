#ifndef H_PARSER_H
#define H_PARSER_H

#include <bkasm.h>
#include <lexer.h>
#include <nodes.h>
#include <cfg_tree.h>

// typedef union ParseResult ParseResult;

STRUCT(Parser)
{
    Cfg_Tree *prog;
    int level;
};

// exports
Parser* parser_create(void);
int parser_init(Parser* parser);
void parser_free(Parser* parser);
void parser_parse(Parser *self, Lexer *lexer);
#endif
