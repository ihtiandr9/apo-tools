
#include <mathexpr.h>
#include <errors.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// forward-decls{

ExprValue *var_resolve(Expr *self);

// }

////////////////////////////////////////////
// ConsExpr

static ExprValue const_evaluate(Expr *self)
{
        return self->data.value;
}

Expr *const_create(ExprValue num)
{
        Expr *expr = (Expr *)malloc(sizeof(Expr));
        expr->type = EXPR_CONST;
        expr->ident = NULL;
        expr->op.evaluate = const_evaluate;
        expr->data.value = num;
        return expr;
}

void const_destroy(Expr *expr)
{
        free(expr->ident);
        free(expr);
}

////////////////////////////////////////////
// Register expression

Expr *register_create(ExprValue reg);

static ExprValue register_evaluate(Expr *self)
{
        return self->data.value;
}

Expr *register_create(ExprValue reg)
{
        Expr *expr = (Expr *)malloc(sizeof(Expr));
        expr->type = EXPR_REG;
        expr->ident = NULL;
        expr->op.evaluate = const_evaluate;
        expr->data.value = reg;
        return (Expr *)expr;
}

void register_destroy(Expr *expr)
{
        free(expr->ident);
        free(expr);
}

////////////////////////////////////////////
// Variable expression

static ExprValue var_evaluate(Expr *self)
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

Expr *var_create(const char *ident)
{

        int len = strlen(ident);
        Expr *expr = (Expr *)malloc(sizeof(Expr));
        expr->type = EXPR_VAR;
        expr->op.evaluate = var_evaluate;
        // props->resolved = 0;
        if (len > MAX_LABEL_SIZE)
                len = MAX_LABEL_SIZE;
        expr->ident = (char *)malloc(len + 1);
        strncpy(expr->ident, ident, len);
        expr->ident[len] = '\0';
        expr->data.value = 0;
        return (Expr *)expr;
}

void var_free(Expr *expr)
{
        if (expr)
        {
                free(expr->ident);
                free(expr);
        }
}

////////////////////////////////////////////
// Math expression

void math_set_lparam(Expr *self, Expr *val)
{
        self->data.mathExpr.lparam = val; 
}

void math_set_rparam(Expr *self, Expr *val)
{
        self->data.mathExpr.rparam = val;
}

ExprOp mathops = {
    NULL, math_set_lparam, math_set_rparam
};

static Expr *math_create(ExprValue opcode)
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

void math_free(Expr *expr)
{
        MathExpr props;
        if (expr)
        {
                switch (expr->type)
                {
                case EXPR_CONST:
                        const_destroy(expr);
                        break;
                case EXPR_REG:
                        register_destroy(expr);
                        break;
                case EXPR_VAR:
                        var_free(expr);
                        break;
                case EXPR_MATH:
                        props = expr->data.mathExpr;
                        math_free(props.lparam);
                        math_free(props.rparam);
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

static ExprValue math_evaluate_multiplication(Expr *self)
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

Expr *math_create_multiplication(ExprValue opcode)
{
        Expr *expr = math_create(opcode);
        expr->op.evaluate = math_evaluate_multiplication;
        return (Expr *)expr;
}

void math_free_multiplication(Expr *expr)
{
        math_free(expr);
}

////////////////////////////////////////////
// Addition expression

static ExprValue math_evaluate_addition(Expr *self)
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

Expr *math_create_addition(ExprValue opcode)
{
        Expr *expr = math_create(opcode);
        expr->op.evaluate = math_evaluate_addition;
        return (Expr *)expr;
}

void math_free_addition(Expr *expr)
{
        math_free(expr);
}
