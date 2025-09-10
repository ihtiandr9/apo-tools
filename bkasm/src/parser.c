#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "bkasm.h"
#include "parser.h"
#include "asmast.h"
#include "errors.h"
#include "inbuf.h"
#include "mathexpr.h"
#include "nodes.h"

static void parse_comment(Parser *self, Lexer *lexer);
static Node *parse_op(Parser *self, Lexer *lexer);
static Expr *parse_param(Parser *self, Lexer *lexer);
static void parse_statement(Parser *self, Lexer *lexer);

static Node *parse_var(Parser *self, Lexer *lexer)
{
    Node *node;

    Lexema m_token = lexer->token;
    char *l_ident = m_token.ident;
    lexer->nextTok(lexer);
    m_token = lexer->token;
    node = NULL;

    if (m_token.type == TOK_COLON)
    {
        node = node_create_label(l_ident);
        lexer->skipOne(lexer);
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        m_token = lexer->token;
        if (m_token.type == TOK_EQU)
        {
            lexer->skipWhile(lexer, ' ');
            lexer->nextTok(lexer);
            node->label.target = parse_param(self, lexer);
            node->label.target_type = TOK_IDENT;
            lexer->skipUntil(lexer, 10);
            parse_comment(self, lexer);
            // pass node throw standart codeflow
        }
        else
        {
            node->label.target = register_create(TOK_REGPC, "PC");
            node->label.target_type = TOK_REGPC;
            ast_add_statement(node, self->ast); // add standart label
            parse_statement(self, lexer);       // parse remain part of string
            node = NULL;                        // nothing to return, all parts were parsed
        }
        free(l_ident);
    }
    else
    {
        throw_error(E_UNKIDENT, l_ident);
        free(l_ident);
        lexer->skipUntil(lexer, 10);
        lexer->skipOne(lexer);
    }
    return node;
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
    Expr *result = NULL;
    char *m_ident = m_token.ident;
    switch (m_token.type)
    {
    case TOK_NUM:
        result = const_create(m_token.value);
        break;
    case TOK_IDENT:
        result = var_create(m_token.ident);
        free(m_ident);
        break;
    default:
        throw_error(E_UNEXPTOKEN, m_token.ident);
        result = const_create(0);
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
    for (; m_token.type == TOK_ASTERISK;)
    {
        Expr *expr = math_create_multiplication(TOK_ASTERISK);
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
    for (; m_token.type == TOK_PLUS || m_token.type == TOK_MINUS;)
    {
        if (m_token.type == TOK_MINUS)
        {
            Expr *expr = math_create_addition(TOK_MINUS);
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
            Expr *expr = math_create_addition(TOK_PLUS);
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
    Expr *expr = NULL;
    Lexema m_token = lexer->token;
    switch (m_token.kind)
    {
    case REG:
        expr = register_create(m_token.type, m_token.ident);
        break;
    case VAR:
    case CONST:
        switch (m_token.type)
        {
        case TOK_IDENT:
        case TOK_NUM:
            expr = parse_addition(self, lexer);
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

static Node *parse_op(Parser *self, Lexer *lexer)
{
    Instruction *op;
    Lexema op_token = lexer->token;
    Node *node = node_create_instruction(op_token.ident, op_token.type);
    InbufCurrentString *currstr = inbuf_currstr();

    switch (op_token.type)
    {
    // multibyte arrays
    case TOK_DB:
        op = (Instruction *)node;
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        op->lparam = parse_param(self, lexer);
        ast_add_statement(node, self->ast);
        lexer->skipWhile(lexer, ',');
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        for (; lexer->token.kind == CONST;)
        {
            node = node_create_instruction(op_token.ident, op_token.type);
            op = (Instruction *)node;
            op->lparam = parse_param(self, lexer);
            ast_add_statement(node, self->ast);
            lexer->skipWhile(lexer, ',');
            lexer->skipWhile(lexer, ' ');
            lexer->nextTok(lexer);
        }
        node = NULL;
        break;

    // two opernds mnemonics
    case TOK_LXI:
    case TOK_MOV:
    case TOK_MVI:
        op = (Instruction *)node;
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
    case TOK_ACI:
    case TOK_ADC:
    case TOK_ADD:
    case TOK_ADI:
    case TOK_ANA:
    case TOK_ANI:
    case TOK_CALL:
    case TOK_CC:
    case TOK_CM:
    case TOK_CMP:
    case TOK_CNC:
    case TOK_CNZ:
    case TOK_CP:
    case TOK_CPE:
    case TOK_CPI:
    case TOK_CPO:
    case TOK_CZ:
    case TOK_DAD:
    case TOK_DCR:
    case TOK_DCX:
    case TOK_IN:
    case TOK_INR:
    case TOK_INX:
    case TOK_JC:
    case TOK_JM:
    case TOK_JMP:
    case TOK_JNC:
    case TOK_JNZ:
    case TOK_JP:
    case TOK_JPE:
    case TOK_JPO:
    case TOK_JZ:
    case TOK_LDA:
    case TOK_LDAX:
    case TOK_LHLD:
    case TOK_ORA:
    case TOK_ORG:
    case TOK_ORI:
    case TOK_OUT:
    case TOK_POP:
    case TOK_PUSH:
    case TOK_RST:
    case TOK_SBB:
    case TOK_SBI:
    case TOK_SHLD:
    case TOK_STA:
    case TOK_STAX:
    case TOK_SUB:
    case TOK_SUI:
    case TOK_XRA:
    case TOK_XRI:
        op = (Instruction *)node;
        lexer->skipWhile(lexer, ' ');
        lexer->nextTok(lexer);
        op->lparam = parse_param(self, lexer);
        break;
    // NULL operand mnemonics
    case TOK_CMA:
    case TOK_CMC:
    case TOK_DAA:
    case TOK_DI:
    case TOK_EI:
    case TOK_END:
    case TOK_HLT:
    case TOK_NOP:
    case TOK_PCHL:
    case TOK_RAL:
    case TOK_RAR:
    case TOK_RC:
    case TOK_RET:
    case TOK_RIM:
    case TOK_RLC:
    case TOK_RM:
    case TOK_RNC:
    case TOK_RNZ:
    case TOK_RP:
    case TOK_RPE:
    case TOK_RPO:
    case TOK_RRC:
    case TOK_RZ:
    case TOK_SIM:
    case TOK_SPHL:
    case TOK_STC:
    case TOK_XCHG:
    case TOK_XTHL:
        op = (Instruction *)node;
        lexer->skipWhile(lexer, ' ');
        break;
    case TOK_SEMICOLON: // no operation pass-throw comment
        break;
    default:
        fprintf(stderr, "In string: %d %s\n", currstr->num, currstr->str);
        throw_error(E_UNKKEYWORD, op_token.ident);
        lexer->skipUntil(lexer, 10);
        break;
    }
    return node;
}

static void parse_statement(Parser *self, Lexer *lexer)
{
    InbufCurrentString *currstr;
    Lexema m_token = lexer->token;
    // lexer->printTok(lexer->token); // debug
    currstr = inbuf_currstr();

    switch (m_token.kind)
    {
    case VAR:
        switch (m_token.type)
        {
        case TOK_IDENT:
            ast_add_statement(parse_var(self, lexer), self->ast);
            break;
        default:
            fprintf(stderr, "In string: %d %s\n", currstr->num, currstr->str);
            throw_error(E_UNEXPTOKEN, m_token.ident);
        }
        break;
    case SYM:
        switch (m_token.type)
        {
        case TOK_SEMICOLON:
            parse_comment(self, lexer);
            break;
        case L_EOL:
            // printf("< EMPTY STRING >\n");
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
        ast_add_statement(parse_op(self, lexer), self->ast);
        lexer->nextTok(lexer);
        parse_comment(self, lexer);
        break;
    case INT:
    {
        Node *internal = node_create_label("ENDPRG");
        m_token = lexer->token;
        if (m_token.type == TOK_END)
        {
            internal->label.target = register_create(TOK_REGPC, "PC");
            internal->label.target_type = TOK_REGPC;
            ast_add_statement(internal, self->ast); // add standart label
            lexer->skipOne(lexer);
            lexer->skipWhile(lexer, ' ');
            lexer->nextTok(lexer);
            parse_statement(self, lexer); // parse remain part of string
            internal = NULL;              // nothing to return, all parts were parsed
        }
    }
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
    for (; lexer->nextTok(lexer);)
    {
        parse_statement(self, lexer);
        lexer->skipWhile(lexer, ' ');
    }
}

int parser_init(Parser *parser)
{
    parser->level = 0;
    parser->ast = ast_create();
    ast_init(parser->ast);
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
