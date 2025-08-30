#ifndef H_PARSER_H
#define H_PARSER_H

//{ forward declarations
STRUCT(Lexer);
STRUCT(ASTree);
//}

STRUCT(Parser)
{
    ASTree *ast;
    int level;
};

// exports
Parser* parser_create(void);
int parser_init(Parser* parser);
void parser_free(Parser* parser);
void parser_parse(Parser *self, Lexer *lexer);
#endif
