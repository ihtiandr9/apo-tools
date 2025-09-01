#ifndef H_NODES_H
#define H_NODES_H
#include <lexer.h>

// { forward decl

typedef union Node Node;
typedef int ExprValue;
STRUCT(Expr);

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
    Expr* immediate;    // lparam or rparam if is math or const or var. set to NULL if is reg
                        // have to be evalued at EVAL_STAGE
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
