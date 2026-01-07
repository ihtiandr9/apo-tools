#ifndef H_NODES_H
#define H_NODES_H
#include <lexer.h>

// { forward decl

// typedef union Node Node;
typedef int ExprValue;
STRUCT(Expr);

// }==================================

typedef enum _NodeType
{
    NODE_EMPTY = 0,
    NODE_INSTRUCTION,
    NODE_VAR,
    NODE_PSEUDO,
} eNodeType;

STRUCT(Instruction)
{
    eIdentType instr_type;
    ExprValue opcode;
    Expr* lparam;
    Expr* rparam;
    Expr* immediate;    // lparam or rparam if is math or const or var. set to NULL if is reg
                        // have to be evalued at EVAL_STAGE
};

STRUCT(Label)
{
    eIdentType target_type;
    Expr* target;
};

STRUCT(Node)
{
    eNodeType type;
    char *ident;
    union
    {
        Instruction op;
        Label label;
    } u;
};

STRUCT(NodeList)
{
    Node node;
    NodeList* next;
};

Node *node_create_label(const char *ident);
void node_clear_label(Label *_label);
Node *node_create_instruction(const char *ident, eIdentType instr_type, ExprValue opcode);
void node_clear_instruction(Instruction *_instr);
void node_clear(Node *node);
void node_print(Node *node);
NodeList *nodelist_alloc();
void nodelist_destroy(NodeList* nodelist);

#endif
