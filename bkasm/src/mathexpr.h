#ifndef H_MATHEXPR_H
#define H_MATHEXPR_H

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

STRUCT(Expr)
{
    eExprType type;
    char *ident;
    ExprOp op; /* virtual methods */
    union
    {
        MathExpr mathExpr;
        ExprValue value;
    } data;
};

Expr *const_create(ExprValue const_value);
Expr *register_create(ExprValue ident_type, const char *ident);
Expr *math_create_addition(ExprValue operation);
Expr *math_create_multiplication(ExprValue operation);
void math_free(Expr *expr);
Expr *var_create(const char *ident);
void math_print_expression(Expr *expr);
#endif
