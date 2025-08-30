#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bkasm.h"
#include "mathexpr.h"
#include "asmast.h"
#include "asmvars.h"
#include "assert.h"
#include "bkasm.h"
#include "errors.h"
#include "lexer.h"

////////////////////////////////////////////
// ConsExpr

static ExprValue const_evaluate(Expr *self)
{
    return self->data.value;
}

Expr *const_create(ExprValue const_value)
{
    Expr *expr = (Expr *)malloc(sizeof(Expr));
    expr->type = EXPR_CONST;
    expr->ident = NULL;
    expr->op.evaluate = const_evaluate;
    expr->data.value = const_value;
    return expr;
}

void const_destroy(Expr *expr)
{
    free(expr->ident);
    free(expr);
}

////////////////////////////////////////////
// Register expression

static ExprValue register_evaluate(Expr *self)
{
    return self->data.value;
}

Expr *register_create(ExprValue ident_type, const char *ident)
{
    Expr *expr = (Expr *)malloc(sizeof(Expr));
    int len = strlen(ident);
    if (expr)
    {
        expr->type = EXPR_REG;
        expr->op.evaluate = register_evaluate;
        if (len > MAX_LABEL_SIZE)
            len = MAX_LABEL_SIZE;
        expr->ident = (char *)malloc(len + 1);
        strncpy(expr->ident, ident, len);
        expr->ident[len] = '\0';
        expr->data.value = ident_type;
    }
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
    char error_msg[100];
    int value;
    if (self->ident)
    {
            if (!asmvars_value(self->ident, &value))
            {
                return value;
            }
            else if (bkasm_stage > PARSE_STAGE)
            {
             
                fprintf(stderr, "Error: undefined variable %s\n", self->ident);
                sprintf(error_msg, "Error: undefined variable %s\n", self->ident);
                throw_error(E_SYNTAXERROR, error_msg);
            }
    }
    return 0;
}

Expr *var_create(const char *ident)
{

    int len = strlen(ident);
    Expr *expr = (Expr *)malloc(sizeof(Expr));
    expr->type = EXPR_VAR;
    expr->op.evaluate = var_evaluate;
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

ExprOp mathops =
{
    NULL, math_set_lparam, math_set_rparam
};

static Expr *math_create(ExprValue opcode)
{
    MathExpr props;
    Expr *expr = (Expr *)malloc(sizeof(Expr));
    if (expr)
    {
        props.opcode = opcode;
        props.lparam = NULL;
        props.rparam = NULL;
        expr->type = EXPR_MATH;
        expr->ident = NULL;
        expr->op = mathops;
        expr->data.mathExpr = props;
    }
    return expr;
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
        if(expr.lparam-> type == EXPR_CONST
                && expr.rparam -> type == EXPR_CONST)
        {
            self -> type = EXPR_CONST;
            self -> op.evaluate = const_evaluate;
            self -> data.value = result;
            math_free(expr.lparam);
            math_free(expr.rparam);
        }
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
    assert(self);
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

Expr *math_create_addition(ExprValue operation)
{
    Expr *expr = math_create(operation);
    expr->op.evaluate = math_evaluate_addition;
    return (Expr *)expr;
}

void math_free_addition(Expr *expr)
{
    math_free(expr);
}

void math_print_operation(ExprValue opcode)
{
    switch (opcode)
    {
    case TOK_ASTERISK:
        printf(INDENT " * ");
        break;

    case TOK_PLUS:
        printf(INDENT " + ");
        break;

    case TOK_MINUS:
        printf(INDENT " - ");
        break;

    default:
        break;
    }
}

void math_print_expression(Expr *expr)
{
    switch (expr->type)
    {
    case EXPR_REG:
        printf(INDENT "< REG >: %s code %d\n", expr->ident,
               expr->data.value);
        break;
    case EXPR_CONST:
        printf(INDENT "< IMMEDIATE >: %d\n", expr->op.evaluate(expr));
        break;
    case EXPR_VAR:
        printf(INDENT "< VAR >: %s\n", expr->ident);
        break;
    case EXPR_MATH:
        printf(INDENT "< MATH >:\n" INDENT);
        math_print_expression(expr->data.mathExpr.lparam);
        math_print_operation(expr->data.mathExpr.opcode);
        math_print_expression(expr->data.mathExpr.rparam);
        break;
    default:
        break;
    }
}
