
#include <mathexpr.h>
#include <nodes.h>
#include <errors.h>
#include <stdlib.h>
#include <assert.h>

////////////////////////////////////////////
// Instruction Node

static ExprValue evaluateInstructionExpr(Node *self)
{
        return self->op.opcode; /* FIXME */
}

Node *createInstruction(ExprValue opcode)
{
        Instruction *node = (Instruction *)malloc(sizeof(Instruction));
        node->type = NODE_INSTRUCTION;
        node->opcode = opcode;
        node->lparam = NULL;
        node->rparam = NULL;
        node->evaluate = evaluateInstructionExpr;
        return (Node *)node;
}

void freeInstruction(Instruction *_instr)
{
        if (_instr)
        {
                freeMathExpr((Math *)_instr->lparam);
                freeMathExpr((Math *)_instr->rparam);
                free(_instr);
        }
}
