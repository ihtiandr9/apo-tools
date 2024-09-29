
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
        Instruction *node = (Instruction *)malloc(sizeof(Instruction));
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

void destroyInstruction(Instruction *_instr)
{
        if (_instr)
        {
                _instr->type = NODE_EMPTY;
                freeMathExpr((Math *)_instr->lparam);
                _instr->lparam = 0;
                freeMathExpr((Math *)_instr->rparam);
                _instr->rparam = 0;
                free(_instr);
        }
}

///////////////////////////////////
// Label Node

Node *createLabel(const char *ident)
{
        int len = strlen(ident);
        Label *node = (Label *)malloc(sizeof(Label));
        node->type = NODE_LABEL;
        node->target = 0;
        if (len > MAX_LABEL_SIZE)
                len = MAX_LABEL_SIZE;
        node->ident = (char *)malloc(len + 1);
        strncpy(node->ident, ident, len);
        node->ident[len] = '\0';
        return (Node *)node;
}

void destroyLabel(Label *_label)
{
        if (_label)
        {
                _label->type = NODE_EMPTY;
                free(_label->ident);
                _label->ident = NULL;
                free(_label);
        }
}
