
#include <mathexpr.h>
#include <errors.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

ExprValue resolveVar(Expr *var);
////////////////////////////////////////////
// ConsExpr

static ExprValue evaluateConstExpr(Expr *self)
{
        return self->value;
}

Expr *createConst(ExprValue num)
{
        Expr *expr = (Expr *)malloc(sizeof(Expr));
        expr->type = EXPR_CONST;
        expr->value = num;
        expr->expr_private = NULL;
        expr->ident = NULL;
        expr->op.evaluate = evaluateConstExpr;
        return expr;
}

void freeConst(Expr *expr)
{
        free(expr->expr_private);
        free(expr->ident);
        free(expr);
}

////////////////////////////////////////////
// Register expression
STRUCT(RegisterPrivate)
{
        eExprType type;
        ExprValue value;
        ExprOp op;
};
Expr *createRegister(ExprValue reg);

static ExprValue evaluateRegtExpr(Expr *self)
{
        return self->value;
}

Expr *createRegister(ExprValue reg)
{
        Expr *expr = (Expr *)malloc(sizeof(Expr));
        expr->type = EXPR_REG;
        expr->value = reg;
        expr->op.evaluate = evaluateConstExpr;
        expr->expr_private = NULL;
        expr->ident = NULL;
        return (Expr *)expr;
}

void freeRegister(Expr *expr)
{
        free(expr->expr_private);
        free(expr->ident);
        free(expr);
}

////////////////////////////////////////////
// Variable expression
STRUCT(VarPrivate)
{
        unsigned char resolved;
};

static ExprValue evaluateVartExpr(Expr *self)
{
        ExprValue result = self->value;
        VarPrivate *props = (VarPrivate *)self->expr_private;
        if (!props->resolved)
                result = resolveVar(self);
        return result;
}

Expr *createVariable(const char *ident)
{

        VarPrivate *props;
		int len = strlen(ident);
        Expr *expr = (Expr *)malloc(sizeof(Expr));
        expr->type = EXPR_VAR;
        expr->value = 0;
        expr->op.evaluate = evaluateVartExpr;
        props = (VarPrivate *)malloc(sizeof(VarPrivate));
        expr->expr_private = props;
        props->resolved = 0;
        if (len > MAX_LABEL_SIZE)
                len = MAX_LABEL_SIZE;
        expr->ident = (char *)malloc(len + 1);
        strncpy(expr->ident, ident, len);
        expr->ident[len] = '\0';
        return (Expr *)expr;
}

void freeVariable(Expr *expr)
{
        if (expr)
        {
                free(expr->ident);
                free(expr->expr_private);
                free(expr);
        }
}

////////////////////////////////////////////
// Math expression

STRUCT(MathPrivate)
{
        Expr *lparam;
        Expr *rparam;
        ExprValue opcode;
};

void MathSetLParam(Expr *self, Expr *val)
{
        MathPrivate *props = (MathPrivate *)self->expr_private;
        props->lparam = val;
}

void MathSetRParam(Expr *self, Expr *val)
{
        MathPrivate *props = (MathPrivate *)self->expr_private;
        props->rparam = val;
}

ExprOp mathops = {
	NULL, MathSetLParam, MathSetRParam
    
};

static Expr *createMathExpr(ExprValue opcode)
{
        MathPrivate *props;
		Expr *expr = (Expr *)malloc(sizeof(Expr));
        expr->type = EXPR_MATH;
        expr->value = 0;
        expr->ident = NULL;
        expr->op = mathops;
        props = (MathPrivate *)malloc(sizeof(MathPrivate));
        expr->expr_private = props;
        props->opcode = opcode;
        props->lparam = NULL;
        props->rparam = NULL;
        return (Expr *)expr;
}

void freeMathExpr(Expr *expr)
{
        MathPrivate *props;
        if (expr)
        {
                switch (expr->type)
                {
                case EXPR_CONST:
                        freeConst(expr);
                        break;
                case EXPR_REG:
                        freeRegister(expr);
                        break;
                case EXPR_VAR:
                        freeVariable(expr);
                        break;
                case EXPR_MATH:
                        props = (MathPrivate *)expr->expr_private;
                        freeMathExpr(props->lparam);
                        freeMathExpr(props->rparam);
                        free(props);
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
        MathPrivate *props = (MathPrivate *)self->expr_private;
        ExprValue result = 0;
        ExprValue lparam = props->lparam->op.evaluate(props->lparam);
        ExprValue rparam = props->rparam->op.evaluate(props->rparam);
        switch (props->opcode)
        {
        case TOK_ASTERISK:
                result = lparam * rparam;
                break;
        default:
                assert(0);
                throw_error(E_SYNTAXERROR, " invalid operation");
                break;
        }
        self->value = result;
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
        MathPrivate *props = (MathPrivate *)self->expr_private;
        ExprValue result = 0;
        ExprValue lparam = props->lparam->op.evaluate(props->lparam);
        ExprValue rparam = props->rparam->op.evaluate(props->rparam);
        switch (props->opcode)
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
        self->value = result;
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
