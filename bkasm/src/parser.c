#include <bkasm.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <cfg_tree.h>
#include <errors.h>
#include <parser.h>
#include <inbuf.h>

static Node* parse_op(Parser *self, Lexer *lexer);

static Node* parse_var(Parser *self, Lexer *lexer)
{
    // Label *target;
    Node *expr;

    Lexema l_token = lexer->token;
    char *l_ident = l_token.ident;
    lexer->nextTok(lexer);
    l_token = lexer->token;
    expr = 0;

    if (l_token.type == TOK_COLON)
    {
        expr = createLabel(l_ident);
        free(l_ident);
        lexer->skipOne(lexer);
    }
    else
    {
        throw_error(E_UNKIDENT, l_ident);
        free(l_ident);
        lexer->skipUntil(lexer, 10);
        lexer->skipOne(lexer);
    }
    return expr;
}

static void parse_comment(Parser *self, Lexer *lexer)
{
    Lexema m_token = lexer->token;
    InbufCurrentString *currstr = inbuf_currstr();
    switch (m_token.kind)
    {
    case SYM:
        switch (m_token.type)
        {
        case L_EOL:
            lexer->skipOne(lexer);
            break;
        case L_EOF:
            lexer->skipOne(lexer);
            return;
        case TOK_SEMICOLON:
            printf("< COMMENT >: skip until eol\n");
            lexer->skipUntil(lexer, 10);
            lexer->skipOne(lexer);
            break;
        default:
            fprintf(stderr, "In string: %d %s\n", currstr->num, currstr->str);
            throw_error(E_UNEXPSYM, m_token.ident);
            lexer->skipUntil(lexer, 10);
        }
        break;
    default:
        fprintf(stderr, "In string:  %d %s\n", currstr->num, currstr->str);
        throw_error(E_UNEXPSYM, m_token.ident);
        lexer->skipUntil(lexer, 10);
        break;
    }
}

static Expr *parse_term(Parser *self, Lexer *lexer)
{
    Lexema m_token = lexer->token;
    Expr *result = 0;
    char *m_ident = m_token.ident;
    switch (m_token.type)
    {
    case TOK_NUM:
        result = createConst(m_token.value);
        break;
    case TOK_IDENT:
        result = createVariable(m_token.ident);
        free(m_ident);
        break;
    default:
        throw_error(E_UNEXPTOKEN, m_token.ident);
        result = createConst(0);
    }
    return result;
}

static Expr *parse_multiplication(Parser *self, Lexer *lexer)
{

    Lexema m_token;
    Expr *result = parse_term(self, lexer);
    lexer->skipWhile(lexer, ' ');
    lexer->nextTok(lexer);
    m_token = lexer->token;
    while (m_token.type == TOK_ASTERISK)
    {
        Expr *expr = createMultiplication(TOK_ASTERISK);
        expr->op.setlparam(expr, result);
        lexer->skipOne(lexer);
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        ;
        expr->op.setrparam(expr, parse_term(self, lexer));
        result = expr;
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        m_token = lexer->token;
    }
    return result;
}

static Expr *parse_addition(Parser *self, Lexer *lexer)
{
    Lexema m_token;
    Expr *result = parse_multiplication(self, lexer);
    lexer->skipWhile(lexer, ' ');
    lexer->nextTok(lexer);
    m_token = lexer->token;
    while (m_token.type == TOK_PLUS || m_token.type == TOK_MINUS)
    {
        if (m_token.type == TOK_MINUS)
        {
            Expr *expr = createAddition(TOK_MINUS);
            expr->op.setlparam(expr, result);
            lexer->skipOne(lexer);
            lexer->skipWhile(lexer, ' ');
            lexer->nextTok(lexer);
            expr->op.setrparam(expr, parse_multiplication(self, lexer));
            result = expr;
            lexer->skipWhile(lexer, ' ');
            lexer->nextTok(lexer);
            m_token = lexer->token;
        }
        if (m_token.type == TOK_PLUS)
        {
            Expr *expr = createAddition(TOK_PLUS);
            expr->op.setlparam(expr, result);
            lexer->skipOne(lexer);
            lexer->skipWhile(lexer, ' ');
            lexer->nextTok(lexer);
            expr->op.setrparam(expr, parse_multiplication(self, lexer));
            result = expr;
            lexer->skipWhile(lexer, ' ');
            lexer->nextTok(lexer);
            m_token = lexer->token;
        }
    }
        return result;
}

static Expr *parse_param(Parser *self, Lexer *lexer)
{
    Expr *expr = 0;
    Lexema m_token = lexer->token;
    switch (m_token.kind)
    {
    case REG:
        expr = createRegister(m_token.type);
        printf(INDENT INDENT "< REGISTER >: %s code %d\n", m_token.ident, expr->op.evaluate(expr));
        break;
    case CONST:
        switch (m_token.type)
        {
        case TOK_NUM:
            expr = parse_addition(self, lexer);
            printf(INDENT INDENT "< IMMEDIATE >: %d\n", expr->op.evaluate(expr));
            break;
        case TOK_IDENT:
            expr = parse_addition(self, lexer);
            printf(INDENT INDENT "< VAR >: %s\n", expr->ident);
            break;
        default:
            throw_error(E_UNKIDENT, m_token.ident);
        }
        break;
    default:
        throw_error(E_UNEXPTOKEN, m_token.ident);
        exit_nicely(E_UNEXPTOKEN);
    }
    lexer->skipWhile(lexer, ' ');
    return expr;
}

static Node* parse_op(Parser *self, Lexer *lexer)
{
    Instruction *op;
    Lexema op_token = lexer->token;
    Node *expr = createInstruction(op_token.ident, op_token.type);
    InbufCurrentString *currstr = inbuf_currstr();

    switch (op_token.type)
    {
    // two opernds mnemonics
    case TOK_MVI:
    case TOK_MOV:
    case TOK_LXI:
        op = (Instruction *)expr;
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        op->lparam = parse_param(self, lexer);
        lexer->skipWhile(lexer, ',');
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        op->rparam = parse_param(self, lexer);
        lexer->skipWhile(lexer, ' ');
        break;
    // one operand mnemonics
    case TOK_JMP:
    case TOK_INR:
    case TOK_DCR:
    case TOK_INX:
    case TOK_DCX:
    case TOK_ORG:
    case TOK_CALL:
        op = (Instruction *)expr;
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        op->lparam = parse_param(self, lexer);
        break;
    case TOK_SEMICOLON: // no operation pass-throw comment
        break;
    default:
        fprintf(stderr, "In string: %d %s\n", currstr->num, currstr->str);
        throw_error(E_UNKKEYWORD, op_token.ident);
        lexer->skipUntil(lexer, 10);
        break;
    }
    return expr;
}

static void parse_statement(Parser *self, Lexer *lexer)
{
    InbufCurrentString *currstr;
    Lexema m_token = lexer->token;
    lexer->printTok(lexer->token); // debug
    currstr = inbuf_currstr();

    switch (m_token.kind)
    {
    case CONST:
        switch (m_token.type)
        {
        case TOK_IDENT:
        cfg_tree_add_statement(parse_var(self, lexer), self->prog);
            
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
            lexer->skipOne(lexer);
            break;
        case L_EOF:
            printf(INDENT "< EOF >\n");
            lexer->skipOne(lexer);
            break;
        default:
            fprintf(stderr, "In string: %d %s\n", currstr->num, currstr->str);
            throw_error(E_UNEXPSYM, m_token.ident);
            exit_nicely(E_UNEXPSYM);
        }
        break;
    case OP:
        cfg_tree_add_statement(parse_op(self, lexer), self->prog);
        lexer->nextTok(lexer);
        parse_comment(self, lexer);
        break;
    case KIND_NONE: // if unexpectd symbol detected
        lexer->skipUntil(lexer, 10);
        lexer->skipOne(lexer);
        break;
    default:
        fprintf(stderr, "In string: %d %s\n", currstr->num, currstr->str);
        throw_error(E_UNEXPTOKEN, m_token.ident);
        assert(0);
        break;
    }
}

void parser_parse(Parser *self, Lexer *lexer)
{
    while (lexer->nextTok(lexer))
    {
        parse_statement(self, lexer);
        lexer->skipWhile(lexer, ' ');
    }
}

int parser_init(Parser *parser)
{
    parser->level = 0;
    parser->prog = cfgtree_create();
    cfgtree_init(parser->prog);
    return 1;
}
Parser *parser_create(void)
{
    Parser *m_parser = (Parser *)malloc(sizeof(Parser));
    parser_init(m_parser);
    return m_parser;
}

void parser_free(Parser *_parser)
{
    free(_parser);
}
