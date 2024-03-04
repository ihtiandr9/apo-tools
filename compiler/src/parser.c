
#include <program.h>
#include <stdlib.h>
#include <globals.h>
#include <errors.h>
#include <stdio.h>
#include <parser.h>

static void parse_op(pParser self, pLexer lexer);

static void parse_var(pParser self, pLexer lexer)
{
    Lexema m_token = lexer->token;
    char *m_ident = m_token.ident;
    lexer->nextTok(lexer);
    m_token = lexer->token;
    if (m_token.type == TOK_COLON)
    {
        printf("< LABEL >: %s\n", m_ident);
        lexer->skipOne(lexer);
        lexer->skipWhile(lexer, ' ');
    }
    else
    {
        throw_error(E_UNKIDENT, m_ident);
        exit_nicely();
    }
}

static void parse_comment(pParser self, pLexer lexer)
{
    Lexema m_token = lexer->token;
    switch (m_token.kind)
    {
    case SYM:
        switch (m_token.type)
        {
        case L_EOL:
            lexer->skipOne(lexer);
            break;
        case L_EOF:
            lexer->ch = TOK_NONE;
            return;
        case TOK_SEMICOLON:
            printf("< COMMENT >: skip until eol\n");
            lexer->skipUntil(lexer, 10);
            lexer->skipOne(lexer);
            break;
        default:
            throw_error(E_UNEXPSYM, m_token.ident);
            lexer->skipUntil(lexer, 10);
        }
        break;
    default:
        throw_error(E_UNEXPSYM, m_token.ident);
        lexer->skipUntil(lexer, 10);
        break;
    }
}

static void parse_const(pParser self, pLexer lexer)
{
    Lexema m_token = lexer->token;
    Node *result = 0;
    if(TOK_NUM == m_token.type)
    {
        result = createConst(m_token.value);
    }else
     {
         throw_error(E_UNEXPTOKEN, m_token.ident);
         result = createConst(0);
     }
    self->statement = result;
}

static void parse_addition(pParser self, pLexer lexer);

static void parse_multiplication(pParser self, pLexer lexer)
{
    Lexema m_token;
    Node *result = 0;
    parse_const(self, lexer);
    result = self->statement;
    lexer->skipWhile(lexer, ' ');
    lexer->nextTok(lexer);
    m_token = lexer->token;
    if (m_token.type == TOK_ASTERISK)
    {
	Multiplication *expr = (Multiplication *)createMultiplication(TOK_ASTERISK);
	expr->lparam = (Const*)result;
	lexer->skipOne(lexer);
	lexer->skipWhile(lexer, ' ');
	lexer->nextTok(lexer);
	parse_multiplication(self, lexer);
	expr->rparam = (Const*)self->statement;
	result = (Node*)expr;
    }
    self->statement = result;
}

static void parse_addition(pParser self, pLexer lexer)
{
    Lexema m_token;
    Node *result = 0;
    parse_multiplication(self, lexer);
    result = self->statement;
    lexer->skipWhile(lexer, ' ');
    lexer->nextTok(lexer);
    m_token = lexer->token;
    if (m_token.type == TOK_MINUS)
    {
        Addition *expr = (Addition *)createAddition(TOK_MINUS);
        expr->lparam = (Const*)result;
        lexer->skipOne(lexer);
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        parse_addition(self, lexer);
        expr->rparam = (Const*)self->statement;
        result = (Node*)expr;
    }
    if (m_token.type == TOK_PLUS)
    {
        Addition *expr = (Addition *)createAddition(TOK_PLUS);
        expr->lparam = (Const*)result;
        lexer->skipOne(lexer);
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        parse_addition(self, lexer);
        expr->rparam = (Const*)self->statement;
        result = (Node*)expr;
    }
    self->statement = result;
}

static void parse_param(pParser self, pLexer lexer)
{
    Node *expr = 0;
    Lexema m_token = lexer->token;
    switch (m_token.kind)
    {
    case REG:
        expr = createRegister(m_token.type);
        printf("        < REGISTER >: %s code %d\n", m_token.ident, expr->num.evaluate((Expr *)expr));
        break;
    case CONST:
        switch (m_token.type)
        {
        case TOK_NUM:
            parse_addition(self, lexer);
            expr = self->statement;
            printf("        < IMMEDIATE >: %d\n", expr->num.evaluate((Expr *)expr));
            break;
        default:
            throw_error(E_UNKIDENT, m_token.ident);
        }
        break;
    default:
        throw_error(E_UNEXPTOKEN, m_token.ident);
    }
    lexer->skipWhile(lexer, ' ');
    self->statement = expr;
}

static void parse_op(pParser self, pLexer lexer)
{
    Lexema op_token = lexer->token;
    Node *expr = createInstruction(op_token.type);
    printf("    < OPERATION >: %s code %d\n", op_token.ident, expr->op.evaluate((InstructionExpr *)expr));

    switch (op_token.type)
    {
    case TOK_MVI:;
        Instruction *op = (Instruction *)expr;
        op->paramCount = 2;
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        parse_param(self, lexer);
        op->lparam = self->statement;
        self->statement = 0;
        lexer->skipWhile(lexer, ',');
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        parse_param(self, lexer);
        lexer->skipWhile(lexer, ' ');
        op->rparam = self->statement;
        self->statement = 0;
        break;
    case TOK_MOV:
        op = (Instruction *)expr;
        op->paramCount = 2;
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        parse_param(self, lexer);
        op->lparam = self->statement;
        self->statement = 0;
        lexer->skipWhile(lexer, ',');
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        parse_param(self, lexer);
        lexer->skipWhile(lexer, ' ');
        op->rparam = self->statement;
        self->statement = 0;
        break;
    case TOK_SEMICOLON: // no operation pass-throw comment
        break;
    default:
        throw_error(E_UNKKEYWORD, op_token.ident);
        lexer->skipUntil(lexer, 10);
        break;
    }
    self->statement = expr;
}

static void parse_statement(pParser self, pLexer lexer)
{
    Lexema m_token = lexer->token;
    self->statement = 0;
    // lexer->printTok(lexer->token); // debug

    switch (m_token.kind)
    {
    case CONST:
        switch (m_token.type)
        {
        case TOK_IDENT:
            parse_var(self, lexer);
            lexer->nextTok(lexer);
            parse_op(self, lexer);
            lexer->nextTok(lexer);
            parse_comment(self, lexer);
            break;
        }
        break;
    case SYM:
        switch (m_token.type)
        {
        case TOK_SEMICOLON:
            parse_comment(self, lexer);
            break;
        case L_EOL:
            printf("< EMPTY STRING >\n");
            lexer->ch = TOK_NONE;
            break;
        case L_EOF:
            printf("    < EOF >\n");
            lexer->ch = TOK_NONE;
            break;
        default:
            throw_error(E_UNEXPSYM, m_token.ident);
            exit_nicely();
        }
        break;
    case OP:
        parse_op(self, lexer);
        lexer->nextTok(lexer);
        parse_comment(self, lexer);
        break;
    default:
        throw_error(E_UNEXPTOKEN, m_token.ident);
        break;
    }
}

static void parser_parse(pParser self, pLexer lexer)
{
    while (lexer->nextTok(lexer))
    {
        if (TOK_SPACE == lexer->token.type)
        {
            lexer->skipWhile(lexer, ' ');
            lexer->nextTok(lexer);
        }
        parse_statement(self, lexer);
        program_generate(self);
    }
}

pParser parser_create(void)
{
    pParser m_parser = (pParser)malloc(sizeof(Parser));
    m_parser->level = 0;
    m_parser->parse = parser_parse;
    return m_parser;
}

void parser_free(pParser _parser)
{
    free(_parser);
}
