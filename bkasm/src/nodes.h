#ifndef H_NODES_H
#define H_NODES_H

#include <bkasm.h>
#include <mathexpr.h>

// { forward decl

typedef union Node Node;

// }==================================

typedef enum _NodeType
{
    NODE_EMPTY = 0,
    NODE_INSTRUCTION,
    NODE_LABEL,
    //NODE_GLOBALVAR,
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

STRUCT(NodeList)
{
    Node node;
    NodeList* next;
};

Node *createLabel(const char *ident);
void clearLabel(Label *_label);
Node *createInstruction(const char *ident, ExprValue opcode);
void clearInstruction(Instruction *_instr);
void clearNode(Node *node);
void nodelist_destroy(NodeList* nodelist);
#endif
