#ifndef H_NODES_H
#define H_NODES_H

#include <globals.h>
#include <mathexpr.h>

// { forward decl

typedef union Node Node;

// }==================================

typedef enum _NodeType
{
    NODE_EMPTY,
    NODE_INSTRUCTION,
    NODE_LABEL,
    NODE_GLOBALVAR,
} eNodeType;

STRUCT(Instruction)
{
    eNodeType type;
    ExprValue opcode;
    ExprValue (*evaluate)(Node *self);
    Expr *lparam;
    Expr *rparam;
};

union Node
{
    eNodeType type;
    Instruction op;
};

Node *createInstruction(ExprValue opcode);
#endif
