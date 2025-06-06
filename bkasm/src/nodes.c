
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
        int len = strlen(ident);
        Instruction *node = (Instruction *)malloc(sizeof(Node));
        node->type = NODE_INSTRUCTION;
        node->opcode = opcode;
        node->lparam = NULL;
        node->rparam = NULL;
        if (len > MAX_LABEL_SIZE)
                len = MAX_LABEL_SIZE;
        node->ident = (char *)malloc(len + 1);
        strncpy(node->ident, ident, len);
        node->ident[len] = '\0';
        return (Node *)node;
}

void node_clear_instruction(Instruction *_instr)
{
        if (_instr)
        {
                _instr->type = NODE_EMPTY;
                free(_instr->ident);
                _instr->ident = NULL;
                math_free((Expr *)_instr->lparam);
                _instr->lparam = 0;
                math_free((Expr *)_instr->rparam);
                _instr->rparam = 0;
        }
}

///////////////////////////////////
// Label Node

Node *node_create_label(const char *ident)
{
        int len = strlen(ident);
        Label *node = (Label *)malloc(sizeof(Node));
        node->type = NODE_LABEL;
        node->target = NULL;
        node->target_type = 0;
        if (len > MAX_LABEL_SIZE)
                len = MAX_LABEL_SIZE;
        node->ident = (char *)malloc(len + 1);
        strncpy(node->ident, ident, len);
        node->ident[len] = '\0';
        return (Node *)node;
}

void node_clear_label(Label *_label)
{
        if (_label)
        {
                _label->type = NODE_EMPTY;
                math_free(_label->target);
                free(_label->ident);
                _label->ident = NULL;
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
        case NODE_LABEL:
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
    while (nodelist)
    {
        next = nodelist->next;
        node_clear(&nodelist->node);
        free(nodelist);
        nodelist = next;
    }
}
