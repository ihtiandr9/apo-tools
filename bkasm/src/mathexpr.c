
#include <mathexpr.h>
#include <errors.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// forward-decls{

ExprValue *resolveVar(Expr *var);

// }

////////////////////////////////////////////
// ConsExpr

static ExprValue evaluateConstExpr(Expr *self)
{
        return self->data.value;
}

Expr *createConst(ExprValue num)
{
        Expr *expr = (Expr *)malloc(sizeof(Expr));
        expr->type = EXPR_CONST;
        expr->ident = NULL;
        expr->op.evaluate = evaluateConstExpr;
        expr->data.value = num;
        return expr;
}

void destroyConst(Expr *expr)
{
        free(expr->ident);
        free(expr);
}

////////////////////////////////////////////
// Register expression

Expr *createRegister(ExprValue reg);

static ExprValue evaluateRegtExpr(Expr *self)
{
        return self->data.value;
}

Expr *createRegister(ExprValue reg)
{
        Expr *expr = (Expr *)malloc(sizeof(Expr));
        expr->type = EXPR_REG;
        expr->ident = NULL;
        expr->op.evaluate = evaluateConstExpr;
        expr->data.value = reg;
        return (Expr *)expr;
}

void destroyRegister(Expr *expr)
{
        free(expr->ident);
        free(expr);
}

////////////////////////////////////////////
// Variable expression

static ExprValue evaluateVartExpr(Expr *self)
{
        /*FIXME
        if (!props->resolved)
        {
                props->valAddr = resolveVar(self);
                if (props->valAddr)
                        props->resolved = 1;
        }*/
        return self->data.value;
}

Expr *createVariable(const char *ident)
{

        int len = strlen(ident);
        Expr *expr = (Expr *)malloc(sizeof(Expr));
        expr->type = EXPR_VAR;
        expr->op.evaluate = evaluateVartExpr;
        // props->resolved = 0;
        if (len > MAX_LABEL_SIZE)
                len = MAX_LABEL_SIZE;
        expr->ident = (char *)malloc(len + 1);
        strncpy(expr->ident, ident, len);
        expr->ident[len] = '\0';
        expr->data.value = 0;
        return (Expr *)expr;
}

void freeVariable(Expr *expr)
{
        if (expr)
        {
                free(expr->ident);
                free(expr);
        }
}

////////////////////////////////////////////
// Math expression

void MathSetLParam(Expr *self, Expr *val)
{
        self->data.mathExpr.lparam = val; 
}

void MathSetRParam(Expr *self, Expr *val)
{
        self->data.mathExpr.rparam = val;
}

ExprOp mathops = {
    NULL, MathSetLParam, MathSetRParam
};

static Expr *createMathExpr(ExprValue opcode)
{
        MathExpr *props;
        Expr *expr = (Expr *)malloc(sizeof(Expr));
        expr->type = EXPR_MATH;
        expr->ident = NULL;
        expr->op = mathops;
        props = &expr->data.mathExpr;
        props->opcode = opcode;
        props->lparam = NULL;
        props->rparam = NULL;
        return (Expr *)expr;
}

void freeMathExpr(Expr *expr)
{
        MathExpr props;
        if (expr)
        {
                switch (expr->type)
                {
                case EXPR_CONST:
                        destroyConst(expr);
                        break;
                case EXPR_REG:
                        destroyRegister(expr);
                        break;
                case EXPR_VAR:
                        freeVariable(expr);
                        break;
                case EXPR_MATH:
                        props = expr->data.mathExpr;
                        freeMathExpr(props.lparam);
                        freeMathExpr(props.rparam);
                        free(expr);
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
        MathExpr expr = self->data.mathExpr;
        ExprValue lparam = expr.lparam->op.evaluate(expr.lparam);
        ExprValue rparam = expr.rparam->op.evaluate(expr.rparam);
        switch (expr.opcode)
        {
        case TOK_ASTERISK:
                result = lparam * rparam;
                break;
        default:
                assert(0);
                throw_error(E_SYNTAXERROR, " invalid operation");
                break;
        }
        return result;
}

Expr *createMultiplication(ExprValue opcode)
{
        Expr *expr = createMathExpr(opcode);
        expr->op.evaluate = evaluateMultiplicationExpr;
        return (Expr *)expr;
}

void freeMultiplication(Expr *expr)
{
        freeMathExpr(expr);
}

////////////////////////////////////////////
// Addition expression

static ExprValue evaluateAdditionExpr(Expr *self)
{
        MathExpr props = self->data.mathExpr;
        ExprValue result = 0;
        ExprValue lparam = props.lparam->op.evaluate(props.lparam);
        ExprValue rparam = props.rparam->op.evaluate(props.rparam);
        switch (props.opcode)
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
        return result;
}

Expr *createAddition(ExprValue opcode)
{
        Expr *expr = createMathExpr(opcode);
        expr->op.evaluate = evaluateAdditionExpr;
        return (Expr *)expr;
}

void freeAddition(Expr *expr)
{
        freeMathExpr(expr);
}
