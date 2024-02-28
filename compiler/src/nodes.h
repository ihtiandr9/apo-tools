#ifndef H_NODES_H
#define H_NODES_H

#include <lexer.h>

typedef int ExprValue;

typedef enum _exprOp
{
    OP_NONE,
    OP_INSTRUCTION,
    OP_CONST,
    OP_REG,
}eExprOp;

typedef struct _Expr
{
    eExprOp type;
    ExprValue (*evaluate)(struct _Expr expr);
    ExprValue value;
} Expr;

typedef struct _InstructionType
{
    eExprOp type;
    ExprValue (*evaluate)(struct _InstructionType expr);
    ExprValue opcode;
} InstructionType;

typedef union _Node
{
    eExprOp type;
    InstructionType op;
    Expr val;
} Node;

typedef struct _RegExpr
{
    Expr super;
} Regexpr;

typedef struct _ConstExpr
{
    Expr super;
} ConstExpr;

typedef struct _Operation
{
    InstructionType super;
    Node lparam;
    Node rparam;
    int paramCount;
} Operation;

#endif
