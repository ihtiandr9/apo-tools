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
    NODE_VAR,
} eNodeType;

STRUCT(Instruction)
{
    eNodeType type;
    eIdentType opcode;
    char *ident;
    Expr* lparam;
    Expr* rparam;
};

STRUCT(Label)
{
    eNodeType type;
    eIdentType target_type;
    char *ident;
    Expr* target;
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

Node *node_create_label(const char *ident);
void node_clear_label(Label *_label);
Node *node_create_instruction(const char *ident, ExprValue opcode);
void node_clear_instruction(Instruction *_instr);
void node_clear(Node *node);
void node_print(Node *node);
NodeList *nodelist_alloc();
void nodelist_destroy(NodeList* nodelist);
#endif
