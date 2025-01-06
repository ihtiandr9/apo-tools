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

Expr *createConst(ExprValue num);
Expr *createRegister(ExprValue reg);
Expr *createAddition(ExprValue operation);
Expr *createMultiplication(ExprValue operation);
void freeMathExpr(Expr *expr);
Expr *createVariable(const char *ident);
#endif
