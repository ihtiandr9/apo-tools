#ifndef H_PARSER_H
#define H_PARSER_H
#include <stdlib.h>
#include <lexer.h>

typedef struct _ValueType
{
    eIdentKind type;
    int value;
} ValueType;

typedef struct _OperationType
{
    eIdentKind type;
    int opcode;
} OperationType;

typedef union _Node
{
    eIdentKind type;
    OperationType op;
    ValueType val;
} Node;

typedef struct _Operation
{
    OperationType super;
    Node lparam;
    Node rparam;
    int paramCount;
} Operation;

typedef struct _Parser
{

    Node *statement;

    void (*parse)(struct _Parser *self, pLexer lexer);

} Parser, *pParser;

pParser parser_create(void);
void parser_free(pParser parser);

#endif
