#include <mathexpr.h>
#include <nodes.h>
#include <errors.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
////////////////////////////////////////////
// Instruction Node

Node *node_create_instruction(const char *ident, ExprValue opcode)
{
    int len;
    Node *node;
    len = strlen(ident);
    if (len > MAX_LABEL_SIZE)
        len = MAX_LABEL_SIZE;
    node = (Node *)malloc(sizeof(Node));
    if(node)
    {
        node->op.type = NODE_INSTRUCTION;
        node->op.opcode = opcode;
        node->op.lparam = NULL;
        node->op.rparam = NULL;
        node->op.ident = (char *)malloc(len + 1);
        strncpy(node->op.ident, ident, len);
        node->op.ident[len] = '\0';
    }
    return (Node *)node;
}

void node_clear_instruction(Instruction *op)
{
    if (op)
    {
        op->type = NODE_EMPTY;
        free(op->ident);
        op->ident = NULL;
        math_free((Expr *)op->lparam);
        op->lparam = 0;
        math_free((Expr *)op->rparam);
        op->rparam = 0;
    }
}

///////////////////////////////////
// Label Node

Node *node_create_label(const char *ident)
{
    int len;
    Label label;
    Node *node;

    len = strlen(ident);
    if (len > MAX_LABEL_SIZE)
        len = MAX_LABEL_SIZE;
    node = (Node *)malloc(sizeof(Node));
    if(node)
    {
        label.type = NODE_VAR;
        label.target = NULL;
        label.target_type = 0;
        label.ident = (char *)malloc(len + 1);
        strncpy(label.ident, ident, len);
        label.ident[len] = '\0';
        node->label = label;
    }
    return (Node *)node;
}

void node_clear_label(Label *label)
{
    if (label)
    {
        label->type = NODE_EMPTY;
        math_free(label->target);
        free(label->ident);
        label->ident = NULL;
    }
}

void node_print(Node *node)
{
    assert(node);
    switch (node->type)
    {
        Instruction instr;
        Label label;
    case NODE_INSTRUCTION:
        instr = node->op;
        printf("< OPERATION >: %s code %d\n", instr.ident,
               instr.opcode);
        if(instr.lparam)
            math_print_expression(instr.lparam);
        if(instr.rparam)
            math_print_expression(instr.rparam);
        break;
    case NODE_VAR:
        label = node->label;
        printf("< LABEL >: %s\n", label.ident);
        math_print_expression(label.target);
        break;
    default:
        assert(0);
        break;
    }
}

void node_clear(Node *node)
{
    if (!node)
        return;
    switch (node->type)
    {
    case NODE_INSTRUCTION:
        node_clear_instruction((Instruction *)node);
        break;
    case NODE_VAR:
        node_clear_label((Label *)node);
        break;
    default:
        assert(0);
        break;
    }
}

NodeList *nodelist_alloc()
{
    NodeList *nodeList;
    nodeList = (NodeList *)malloc(sizeof(NodeList));
    nodeList->next = 0;
    nodeList->node.type = NODE_EMPTY;
    return nodeList;
}

void nodelist_destroy(NodeList *nodelist)
{
    NodeList *next = 0;
    for (; nodelist; nodelist = next)
    {
        next = nodelist->next;
        node_clear(&nodelist->node);
        free(nodelist);
    }
}
