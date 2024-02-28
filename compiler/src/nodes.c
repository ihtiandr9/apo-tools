
#include <nodes.h>
#include <stdlib.h>
Node *createRegister(ExprValue reg)
{
        Register *expr = (Register *)malloc(sizeof(Register));
        expr->super.type = OP_REG;
        expr->super.value = reg;
        return (Node*)expr;
}

Node *createConstExpr(ExprValue num)
{
        ConstExpr *expr = (ConstExpr *)malloc(sizeof(ConstExpr));
        expr->super.type = OP_CONST;
        expr->super.value = num;
        return (Node*)expr;
}

Node *createInstruction(ExprValue opcode)
{
        Instruction *expr = (Instruction *)malloc(sizeof(Instruction));
        expr->super.type = OP_INSTRUCTION;
        expr->super.opcode = opcode;
        return (Node*)expr;
}
