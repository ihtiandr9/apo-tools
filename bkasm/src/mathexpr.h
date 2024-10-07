#ifndef H_MATHEXPR_H
#define H_MATHEXPR_H

#include <lexer.h>

// { forward decl

typedef union Expr Expr;

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

STRUCT(Const)
{
    eExprType type;
    ExprValue value;
    ExprValue (*evaluate)(Expr *self);
};

STRUCT(Register)
{
    eExprType type;
    ExprValue value;
    ExprValue (*evaluate)(Expr *self);
};

STRUCT(Variable)
{
    eExprType type;
    ExprValue value;
    ExprValue (*evaluate)(Expr *self);
    char* ident;
    unsigned char resolved;
};

STRUCT(Math)
{
    eExprType type;
    ExprValue result;
    ExprValue (*evaluate)(Expr *self);
    Expr *lparam;
    Expr *rparam;
    ExprValue opcode;
};

union Expr
{
    eExprType type;
    Const constval;
    Register reg;
    Math mathExpr;
    Variable var;
};

Expr *createRegister(ExprValue reg);
Expr *createConst(ExprValue num);
Expr *createAddition(ExprValue operation);
Expr *createMultiplication(ExprValue operation);
Expr *createVariable(const char *ident);
void freeMathExpr(Math *expr);
#endif
