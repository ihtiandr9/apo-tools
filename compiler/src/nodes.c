
#include <nodes.h>
#include <errors.h>
#include <stdlib.h>

////////////////////////////////////////////
// ConsExpr

static ExprValue evaluateConstExpr(Expr *self)
{
        return self->value;
}

Node *createConst(ExprValue num)
{
        Const *expr = (Const *)malloc(sizeof(Const));
        expr->super.type = NODE_CONST;
        expr->super.value = num;
        expr->super.evaluate = evaluateConstExpr;
        return (Node *)expr;
}

void freeConst(Node *node)
{
        if (node)
        {
                free(node);
        }
}

////////////////////////////////////////////
// Register expression

Node *createRegister(ExprValue reg)
{
        Register *expr = (Register *)malloc(sizeof(Register));
        expr->super.type = NODE_REG;
        expr->super.value = reg;
        expr->super.evaluate = evaluateConstExpr;
        return (Node *)expr;
}

void freeRegister(Node *node)
{
        if (node)
        {
                free(node);
        }
}

////////////////////////////////////////////
// Instruction expression

static ExprValue evaluateInstructionExpr(InstructionExpr *self)
{
        /* FIXME */
        return self->opcode;
}

Node *createInstruction(ExprValue opcode)
{
        Instruction *expr = (Instruction *)malloc(sizeof(Instruction));
        expr->super.type = NODE_INSTRUCTION;
        expr->super.opcode = opcode;
        expr->super.evaluate = evaluateInstructionExpr;
        return (Node *)expr;
}

void freeInstruction(Node *node)
{
        if (node)
        {
                Instruction *m_instr = (Instruction *)node;
                freeConst((Node *)m_instr->lparam);
                freeConst((Node *)m_instr->rparam);
                free(m_instr);
        }
}


////////////////////////////////////////////
// Multiply expression

static ExprValue evaluateMultiplicationExpr(InstructionExpr *self)
{
	ExprValue result = 0;
	Multiplication *expr = (Multiplication *)self;
	ExprValue lparam = expr->lparam->super.evaluate((Expr *)(expr->lparam));
	ExprValue rparam = expr->rparam->super.evaluate((Expr *)(expr->rparam));
	if (expr->super.opcode == TOK_ASTERISK)
		result = lparam * rparam;
	else
		throw_error(E_SYNTAXERROR, " invalid operation");
	return result;
}

Node *createMultiplication(ExprValue opcode)
{
	Multiplication *expr = (Multiplication *)malloc(sizeof(Addition));
	expr->super.type = NODE_MATH;
	expr->super.opcode = opcode;
	expr->super.evaluate = evaluateMultiplicationExpr;
	expr->lparam = 0;
	expr->rparam = 0;
	return (Node *)expr;
}

void freeMultiplication(Node *node)
{
	if (node)
	{
		Multiplication *m_instr = (Multiplication *)node;
		freeConst((Node *)m_instr->lparam);
		freeConst((Node *)m_instr->rparam);
		free(m_instr);
	}
}

////////////////////////////////////////////
// Addition expression

static ExprValue evaluateAdditionExpr(InstructionExpr *self)
{
        ExprValue result = 0;
        Addition *expr = (Addition *)self;
        ExprValue lparam = expr->lparam->super.evaluate((Expr *)(expr->lparam));
        ExprValue rparam = expr->rparam->super.evaluate((Expr *)(expr->rparam));
        if (expr->super.opcode == TOK_PLUS)
                result = lparam + rparam;
        else if (expr->super.opcode == TOK_MINUS)
                result = lparam - rparam;
        else
                throw_error(E_SYNTAXERROR, " invalid operation");
        return result;
}

Node *createAddition(ExprValue opcode)
{
        Addition *expr = (Addition *)malloc(sizeof(Addition));
        expr->super.type = NODE_MATH;
        expr->super.opcode = opcode;
        expr->super.evaluate = evaluateAdditionExpr;
        expr->lparam = 0;
        expr->rparam = 0;
        return (Node *)expr;
}

void freeAddition(Node *node)
{
        if (node)
        {
                Addition *m_instr = (Addition *)node;
                freeConst((Node *)m_instr->lparam);
                freeConst((Node *)m_instr->rparam);
                free(m_instr);
        }
}
