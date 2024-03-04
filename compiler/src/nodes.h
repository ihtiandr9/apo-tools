#ifndef H_NODES_H
#define H_NODES_H

#include <lexer.h>

typedef int ExprValue;

typedef enum _NodeType
{
    NODE_EMPTY,
    NODE_INSTRUCTION,
    NODE_CONST,
    NODE_REG,
    NODE_MATH
} eNodeType;

typedef struct _Expr
{
    eNodeType type;
    ExprValue value;
    ExprValue (*evaluate)(struct _Expr *self);
} Expr;

typedef struct _InstructionExpr
{
    eNodeType type;
    ExprValue opcode;
    ExprValue (*evaluate)(struct _InstructionExpr *self);
} InstructionExpr;

typedef union _Node
{
    eNodeType type;
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
    Node *lparam;
    Node *rparam;
    int paramCount;
} Instruction;

typedef struct _Addition
{
    InstructionExpr super;
    Const *lparam;
    Const *rparam;
} Addition;

typedef struct _Multiplication
{
    InstructionExpr super;
    Const *lparam;
    Const *rparam;
} Multiplication;

Node *createRegister(ExprValue reg);
Node *createConst(ExprValue num);
Node *createInstruction(ExprValue opcode);
Node *createAddition(ExprValue operation);
Node *createMultiplication(ExprValue operation);
#endif
