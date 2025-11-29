#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bkasm.h"
#include "nodes.h"
#include "errors.h"
#include "lexer.h"
#include "mathexpr.h"
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
        node->type = NODE_INSTRUCTION;
        node->u.op.opcode = opcode;
        node->u.op.lparam = NULL;
        node->u.op.rparam = NULL;
        node->u.op.immediate = NULL;
        node->ident = (char *)malloc(len + 1);
        strncpy(node->ident, ident, len);
        node->ident[len] = '\0';
    }
    return (Node *)node;
}

void node_clear_instruction(Instruction *op)
{
    if (op)
    {
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
    Node *node;

    len = strlen(ident);
    if (len > MAX_LABEL_SIZE)
        len = MAX_LABEL_SIZE;
    node = (Node *)malloc(sizeof(Node));
    if(node)
    {
        node->type = NODE_VAR;
        node->u.label.target = NULL;
        node->u.label.target_type = 0;
        node->ident = (char *)malloc(len + 1);
        strncpy(node->ident, ident, len);
        node->ident[len] = '\0';
    }
    return (Node *)node;
}

void node_clear_label(Label *label)
{
    if (label)
    {
        math_free(label->target);
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
        instr = node->u.op;
        printf("< OPERATION >: %s code %d\n", node->ident,
               instr.opcode);
        if(instr.lparam)
            math_print_expression(instr.lparam);
        if(instr.rparam)
            math_print_expression(instr.rparam);
        break;
    case NODE_VAR:
        label = node->u.label;
        printf("< LABEL >: %s\n", node->ident);
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
        node_clear_instruction(&node->u.op);
        break;
    case NODE_VAR:
        node_clear_label(&node->u.label);
        break;
    default:
        assert(0);
        break;
    }
    if (node->ident)
    {
        free(node->ident);
        node->ident = 0;
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
