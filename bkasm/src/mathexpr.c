
#include <mathexpr.h>
#include <errors.h>
#include <stdlib.h>
#include <assert.h>

////////////////////////////////////////////
// ConsExpr

static ExprValue evaluateConstExpr(Expr *self)
{
        return self->constval.value;
}

Expr *createConst(ExprValue num)
{
        Const *expr = (Const *)malloc(sizeof(Const));
        expr->type = EXPR_CONST;
        expr->value = num;
        expr->evaluate = evaluateConstExpr;
        return (Expr *)expr;
}

void freeConst(Const *expr)
{
        if (expr)
        {
                free(expr);
        }
}

////////////////////////////////////////////
// Register expression

static ExprValue evaluateRegtExpr(Expr *self)
{
        return self->reg.value;
}

Expr *createRegister(ExprValue reg)
{
        Register *expr = (Register *)malloc(sizeof(Expr));
        expr->type = EXPR_REG;
        expr->value = reg;
        expr->evaluate = evaluateConstExpr;
        return (Expr *)expr;
}

void freeRegister(Register *expr)
{
        if (expr)
        {
                free(expr);
        }
}

////////////////////////////////////////////
// Math expression

static Expr *createMathExpr(ExprValue opcode)
{
        Math *expr = (Math *)malloc(sizeof(Math));
        expr->type = EXPR_MATH;
        expr->opcode = opcode;
        expr->lparam = NULL;
        expr->rparam = NULL;
        expr->result = 0;
        return (Expr *)expr;
}

void freeMathExpr(Math *expr)
{
	Math *m_instr=0;
        if (expr)
        {
                switch (expr->type)
                {
                case EXPR_CONST:
                        freeConst((Const *)expr);
                        break;
                case EXPR_REG:
                        freeRegister((Register *)expr);
                        break;
                case EXPR_MATH:
                        m_instr = (Math *)expr;
                        freeMathExpr((Math *)m_instr->lparam);
                        freeMathExpr((Math *)m_instr->rparam);
                        free(m_instr);
                        break;
                default:
                        assert(0);
                        break;
                }
        }
}

////////////////////////////////////////////
// Multiply expression

static ExprValue evaluateMultiplicationExpr(Expr *self)
{
        ExprValue result = 0;
        ExprValue lparam = self->mathExpr.lparam->mathExpr.evaluate((Expr *)(self->mathExpr.lparam));
        ExprValue rparam = self->mathExpr.rparam->mathExpr.evaluate((Expr *)(self->mathExpr.rparam));
        switch (self->mathExpr.opcode)
        {
        case TOK_ASTERISK:
                result = lparam * rparam;
                break;
        default:
                assert(0);
                throw_error(E_SYNTAXERROR, " invalid operation");
                break;
        }
        self->mathExpr.result = result;
        return result;
}

Expr *createMultiplication(ExprValue opcode)
{
        Expr *expr = createMathExpr(opcode);
        expr->mathExpr.evaluate = evaluateMultiplicationExpr;
        return (Expr *)expr;
}

void freeMultiplication(Math *expr)
{
        freeMathExpr(expr);
}

////////////////////////////////////////////
// Addition expression

static ExprValue evaluateAdditionExpr(Expr *self)
{
        ExprValue result = 0;
        ExprValue lparam = self->mathExpr.lparam->mathExpr.evaluate((Expr *)(self->mathExpr.lparam));
        ExprValue rparam = self->mathExpr.rparam->mathExpr.evaluate((Expr *)(self->mathExpr.rparam));
        switch (self->mathExpr.opcode)
        {
        case TOK_PLUS:
                result = lparam + rparam;
                break;
        case TOK_MINUS:
                result = lparam - rparam;
                break;
        default:
                assert(0);
                throw_error(E_SYNTAXERROR, " invalid operation");
                break;
        }
        self->mathExpr.result = result;
        return result;
}

Expr *createAddition(ExprValue opcode)
{
        Expr *expr = createMathExpr(opcode);
        expr->mathExpr.evaluate = evaluateAdditionExpr;
        return (Expr *)expr;
}

void freeAddition(Math *expr)
{
        freeMathExpr(expr);
}
