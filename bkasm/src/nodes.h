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
    int opcode;
    char *ident;
    Expr* lparam;
    Expr* rparam;
};

STRUCT(Label)
{
    eNodeType type;
    int target;
    char *ident;
};

union Node
{
    eNodeType type;
    Instruction op;
    Label label;
};

Node *createLabel(const char *ident);
void destroyLabel(Label *_label);
Node *createInstruction(const char *ident, ExprValue opcode);
void destroyInstruction(Instruction *_instr);
#endif
