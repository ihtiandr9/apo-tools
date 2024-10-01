
#include <mathexpr.h>
#include <nodes.h>
#include <errors.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
////////////////////////////////////////////
// Instruction Node

Node *createInstruction(const char *ident, ExprValue opcode)
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

void clearInstruction(Instruction *_instr)
{
        if (_instr)
        {
                _instr->type = NODE_EMPTY;
                free(_instr->ident);
                _instr->ident = NULL;
                freeMathExpr((Math *)_instr->lparam);
                _instr->lparam = 0;
                freeMathExpr((Math *)_instr->rparam);
                _instr->rparam = 0;
        }
}

///////////////////////////////////
// Label Node

Node *createLabel(const char *ident)
{
        int len = strlen(ident);
        Label *node = (Label *)malloc(sizeof(Node));
        node->type = NODE_LABEL;
        node->target = 0;
        if (len > MAX_LABEL_SIZE)
                len = MAX_LABEL_SIZE;
        node->ident = (char *)malloc(len + 1);
        strncpy(node->ident, ident, len);
        node->ident[len] = '\0';
        return (Node *)node;
}

void clearLabel(Label *_label)
{
        if (_label)
        {
                _label->type = NODE_EMPTY;
                free(_label->ident);
                _label->ident = NULL;
        }
}

void clearNode(Node *node)
{
        if (!node)
                return;
        switch (node->type)
        {
        case NODE_INSTRUCTION:
                clearInstruction((Instruction *)node);
                break;
        case NODE_LABEL:
                clearLabel((Label *)node);
                break;
        default:
                assert(0);
                break;
        }
}