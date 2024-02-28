#ifndef H_PARSER_H
#define H_PARSER_H
#include <stdlib.h>
#include <lexer.h>

typedef struct _Node {
    eIdentType type;
    int value;
} Node;

typedef struct _Operation{
    Node super;
    int paramCount;
}Operation;

typedef struct _Program {
     Operation *operation;
     Operation *next;
}Program;

typedef struct _Parser{

    Node *statement;

    void (*parse)(struct _Parser *self, pLexer lexer);

}Parser, *pParser;

pParser parser_create(void);
void parser_free(pParser parser);

#endif
