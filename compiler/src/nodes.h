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
    OP_ADDITIVE
} eExprOp;

typedef struct _Expr
{
    eExprOp type;
    ExprValue value;
    ExprValue (*evaluate)(struct _Expr *self);
} Expr;

typedef struct _InstructionExpr
{
    eExprOp type;
    ExprValue opcode;
    ExprValue (*evaluate)(struct _InstructionExpr *self);
} InstructionExpr;

typedef union _Node
{
    eExprOp type;
    InstructionExpr op;
    Expr num;
} Node;

typedef struct _Register
{
    Expr super;
} Register;

typedef struct _Const
{
    Expr super;
} Const;

typedef struct _Instruction
{
    InstructionExpr super;
    Node lparam;
    Node rparam;
    int paramCount;
} Instruction;

typedef struct _Addition
{
    InstructionExpr super;
    Node lparam;
    Node rparam;
} Addition;

Node *createRegister(ExprValue reg);
Node *createConst(ExprValue num);
Node *createInstruction(ExprValue opcode);
Node *createAddition(ExprValue lparam);
#endif
