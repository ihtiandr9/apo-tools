#ifndef H_MATHEXPR_H
#define H_MATHEXPR_H

#include <lexer.h>

// { forward decl

STRUCT(Expr);

// }==================================
typedef int ExprValue;

typedef enum _ExprType
{
    EXPR_EMPTY,
    EXPR_CONST,
    EXPR_REG,
    EXPR_VAR,
    EXPR_MATH
} eExprType;

STRUCT(ExprOp)
{
    ExprValue (*evaluate)(Expr *self);
    void (*setlparam)(Expr *self, Expr *val);
    void (*setrparam)(Expr *self, Expr *val);
};

STRUCT(MathExpr)
{
    Expr *lparam;
    Expr *rparam;
    ExprValue opcode;
};

STRUCT(Variable)
{
    ExprValue dirty;
    ExprValue *ref;
};

STRUCT(Expr)
{
    eExprType type;
    char *ident;
    ExprOp op;
    union
    {
        MathExpr mathExpr;
        ExprValue value;
    } data;
};

Expr *const_create(ExprValue num);
Expr *register_create(ExprValue reg);
Expr *math_create_addition(ExprValue operation);
Expr *math_create_multiplication(ExprValue operation);
void math_free(Expr *expr);
Expr *var_create(const char *ident);
#endif
