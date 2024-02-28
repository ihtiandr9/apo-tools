
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
        lexer->skipWhile(lexer, ':');
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
        case L_EOF:
            lexer->ch = TOK_NONE;
            return;
        case TOK_SEMICOLON:
            printf("< COMMENT >: skip until eol\n");
            lexer->skipUntil(lexer, 10);
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

static void parse_param(pParser self, pLexer lexer)
{
    Node *expr = 0;
    Lexema m_token = lexer->token;
    switch (m_token.kind)
    {
    case REG:
        printf("    < REGISTER >: %s\n", m_token.ident);
        /* FIXME */
        // expr = createRegexpr));
        expr = (Node *)malloc(sizeof(Regexpr));
        expr->type = REG;
        expr->val.value = m_token.type;
        break;
    case CONST:
        switch (m_token.type)
        {
        case TOK_NUM:
            printf("    < VALUE >: %d\n", m_token.value);
            /* FIXME */
            // expr = createPrimaryNumExpr));
            expr = (Node *)malloc(sizeof(ConstExpr));
            expr->type = OP_CONST;
            expr->val.value = m_token.value;
            break;
        default:
            throw_error(E_UNKIDENT, m_token.ident);
        }
        break;
    default:
        throw_error(E_UNEXPTOKEN, m_token.ident);
    }
    self->statement = expr;
}

static void parse_op(pParser self, pLexer lexer)
{
    Lexema op_token = lexer->token;
    /* FIXME */
    // Node *op = 0
    Node *expr = (Node *)malloc(sizeof(Operation));
    expr->type = OP_INSTRUCTION;
    expr->op.opcode = op_token.type;
    printf("    < OPERATION >: %s\n", op_token.ident);

    switch (op_token.type)
    {
    case TOK_MVI:;
        Operation *op = (Operation *)expr;
        op->paramCount = 2;
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        parse_param(self, lexer);
        op->lparam = *(self->statement);
        free(self->statement);
        self->statement = 0;
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        lexer->skipWhile(lexer, ',');
        lexer->nextTok(lexer);
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        parse_param(self, lexer);
        op->rparam = *(self->statement);
        free(self->statement);
        break;
    case TOK_MOV:
        op = (Operation *)expr;
        op->paramCount = 2;
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        parse_param(self, lexer);
        op->lparam = *(self->statement);
        free(self->statement);
        self->statement = 0;
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        lexer->skipWhile(lexer, ',');
        lexer->nextTok(lexer);
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        parse_param(self, lexer);
        if (self->statement)
            op->rparam = *(self->statement);
        free(self->statement);
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
            lexer->skipWhile(lexer, ' ');
            lexer->nextTok(lexer);
            parse_op(self, lexer);
            lexer->skipWhile(lexer, ' ');
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
            printf("    < EOL >\n");
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
    m_parser->parse = parser_parse;
    return m_parser;
}

void parser_free(pParser _parser)
{
    free(_parser);
}
