
#include <nodes.h>
#include <stdlib.h>

////////////////////////////////////////////
// ConsExpr

static ExprValue evaluateConstExpr(Expr *self)
{
        return self->value;
}

Node *createConst(ExprValue num)
{
        Const *expr = (Const *)malloc(sizeof(Const));
        expr->super.type = OP_CONST;
        expr->super.value = num;
        expr->super.evaluate = evaluateConstExpr;
        return (Node *)expr;
}

////////////////////////////////////////////
// Register expression

Node *createRegister(ExprValue reg)
{
        Register *expr = (Register *)malloc(sizeof(Register));
        expr->super.type = OP_REG;
        expr->super.value = reg;
        expr->super.evaluate = evaluateConstExpr;
        return (Node *)expr;
}

////////////////////////////////////////////
// Instruction expression

static ExprValue evaluateInstructionExpr(InstructionExpr *self)
{
        return self->opcode;
}

Node *createInstruction(ExprValue opcode)
{
        Instruction *expr = (Instruction *)malloc(sizeof(Instruction));
        expr->super.type = OP_INSTRUCTION;
        expr->super.opcode = opcode;
        expr->super.evaluate = evaluateInstructionExpr;
        return (Node *)expr;
}

////////////////////////////////////////////
// Addition expression

static ExprValue evaluateAdditionExpr(Expr *self)
{
        Addition *expr = (Addition *)self;
        return 0;
}

Node *createAddition(ExprValue lparam)
{
        Addition *expr = (Addition *)malloc(sizeof(Addition));
        expr->super.type = OP_ADDITIVE;
        expr->super.opcode = 0; // op
        expr->lparam.type = OP_CONST;
        expr->lparam.num.value = lparam;
        expr->super.evaluate = evaluateInstructionExpr;
        return (Node *)expr;
}
