#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bkasm.h"
#include "lexer.h"
#include "bkints.h"
#include "errors.h"
#include "inbuf.h"
#include "symbols.h"

// imports
extern const char eof_sym;
extern const char eol_sym;

static const Lexema words[] =
{
    {OP, TOK_CC, "CC", 0xDC, 2},
    {OP, TOK_CM, "CM", 0xFC, 2},
    {OP, TOK_CP, "CP", 0xF4, 2},
    {OP, TOK_CZ, "CZ", 0xCC, 2},
    {OP, TOK_DB, "DB", 0, 2},
    {OP, TOK_DI, "DI", 0xF3, 2},
    {OP, TOK_EI, "EI", 0xFB, 2},
    {OP, TOK_IN, "IN", 0xDB, 2},
    {OP, TOK_JC, "JC", 0xDA, 2},
    {OP, TOK_JM, "JM", 0xFA, 2},
    {OP, TOK_JP, "JP", 0xF2, 2},
    {OP, TOK_JZ, "JZ", 0xCA, 2},
    {OP, TOK_RC, "RC", 0xD8, 2},
    {OP, TOK_RM, "RM", 0xF8, 2},
    {OP, TOK_RP, "RP", 0xF0, 2},
    {OP, TOK_RZ, "RZ", 0xC8, 2},
    {OP, TOK_ACI, "ACI", 0xCE, 3},
    {OP, TOK_ADC, "ADC", 0x88, 3},
    {OP, TOK_ADD, "ADD", 0x80, 3},
    {OP, TOK_ADI, "ADI", 0xC6, 3},
    {OP, TOK_ANA, "ANA", 0, 3},
    {OP, TOK_AND, "AND", 0, 3},
    {OP, TOK_ANI, "ANI", 0, 3},
    {OP, TOK_CMA, "CMA", 0, 3},
    {OP, TOK_CMC, "CMC", 0, 3},
    {OP, TOK_CMP, "CMP", 0, 3},
    {OP, TOK_CNC, "CNC", 0, 3},
    {OP, TOK_CNZ, "CNZ", 0, 3},
    {OP, TOK_CPE, "CPE", 0, 3},
    {OP, TOK_CPI, "CPI", 0, 3},
    {OP, TOK_CPO, "CPO", 0, 3},
    {OP, TOK_DAA, "DAA", 0, 3},
    {OP, TOK_DAD, "DAD", 0, 3},
    {OP, TOK_DCR, "DCR", 0, 3},
    {OP, TOK_DCX, "DCX", 0, 3},
    {OP, TOK_EQU, "EQU", 0, 3},
    {OP, TOK_HLT, "HLT", 0, 3},
    {OP, TOK_INR, "INR", 0, 3},
    {OP, TOK_INX, "INX", 0, 3},
    {OP, TOK_JMP, "JMP", 0xC3, 3},
    {OP, TOK_JNC, "JNC", 0, 3},
    {OP, TOK_JNZ, "JNZ", 0, 3},
    {OP, TOK_JPE, "JPE", 0, 3},
    {OP, TOK_JPO, "JPO", 0, 3},
    {OP, TOK_LDA, "LDA", 0x3A, 3},
    {OP, TOK_LXI, "LXI", 0x1, 3},
    {OP, TOK_MOV, "MOV", 0x40, 3},
    {OP, TOK_MVI, "MVI", 0x6, 3},
    {OP, TOK_NOP, "NOP", 0, 3},
    {OP, TOK_ORA, "ORA", 0, 3},
    {OP, TOK_ORG, "ORG", 0, 3},
    {OP, TOK_ORI, "ORI", 0, 3},
    {OP, TOK_OUT, "OUT", 0, 3},
    {OP, TOK_POP, "POP", 0, 3},
    {OP, TOK_RAL, "RAL", 0, 3},
    {OP, TOK_RAR, "RAR", 0, 3},
    {OP, TOK_RET, "RET", 0xC9, 3},
    {OP, TOK_RIM, "RIM", 0, 3},
    {OP, TOK_RLC, "RLC", 0, 3},
    {OP, TOK_RNC, "RNC", 0, 3},
    {OP, TOK_RNZ, "RNZ", 0xC0, 3},
    {OP, TOK_RPE, "RPE", 0, 3},
    {OP, TOK_RPO, "RPO", 0, 3},
    {OP, TOK_RRC, "RRC", 0, 3},
    {OP, TOK_RST, "RST", 0, 3},
    {OP, TOK_SBB, "SBB", 0, 3},
    {OP, TOK_SBI, "SBI", 0, 3},
    {OP, TOK_SIM, "SIM", 0, 3},
    {OP, TOK_STA, "STA", 0x32, 3},
    {OP, TOK_STC, "STC", 0, 3},
    {OP, TOK_SUB, "SUB", 0, 3},
    {OP, TOK_SUI, "SUI", 0, 3},
    {OP, TOK_XRA, "XRA", 0, 3},
    {OP, TOK_XRI, "XRI", 0, 3},
    {OP, TOK_CALL, "CALL", 0xCD, 4},
    {OP, TOK_LDAX, "LDAX", 0x0A, 4},
    {OP, TOK_LHLD, "LHLD", 0x2A, 4},
    {OP, TOK_PCHL, "PCHL", 0xE9, 4},
    {OP, TOK_PUSH, "PUSH", 0, 4},
    {OP, TOK_SHLD, "SHLD", 0x22, 4},
    {OP, TOK_SPHL, "SPHL", 0xF9, 4},
    {OP, TOK_STAX, "STAX", 0x02, 4},
    {OP, TOK_XCHG, "XCHG", 0xEB, 4},
    {OP, TOK_XTHL, "XTHL", 0, 4},
    {REG, TOK_REGA, "A", 0x7, 1},
    {REG, TOK_REGB, "B", 0x0, 1},
    {REG, TOK_REGC, "C", 0x1, 1},
    {REG, TOK_REGD, "D", 0x2, 1},
    {REG, TOK_REGE, "E", 0x3, 1},
    {REG, TOK_REGH, "H", 0x4, 1},
    {REG, TOK_REGL, "L", 0x5, 1},
    {REG, TOK_REGM, "M", 0x6, 1},
    {REG, TOK_REGBC, "BC", 0x0, 2},
    {REG, TOK_REGDE, "DE", 0x1, 2},
    {REG, TOK_REGHL, "HL", 0x2, 2},
    {REG, TOK_REGSP, "SP", 0x3, 2},
    {INT, TOK_END, "END", 0, 3},
    {KIND_NONE, TOK_NONE, 0, 0, 0},
};

static const Lexema symbols[] =
{
    {SYM, TOK_COLON, ":", 0, 1},
    {SYM, TOK_SEMICOLON, ";", 0, 1},
    {SYM, TOK_COMMA, ",", 0, 1},
    {SYM, TOK_EQ, "=", 0, 1},
    {SYM, TOK_GT, ">", 0, 1},
    {SYM, TOK_LT, "<", 0, 1},
    {SYM, TOK_PLUS, "+", 0, 1},
    {SYM, TOK_MINUS, "-", 0, 1},
    {SYM, TOK_ASTERISK, "*", 0, 1},
    {SYM, L_EOL, (char *)&eol_sym, 0, 1},
    {SYM, L_EOF, (char *)&eof_sym, 0, 1},
    {KIND_NONE, TOK_NONE, 0, 0, 0},
};

static int lexer_next_tok(Lexer *self)
{
    char m_ch = self->ch;
    int f_result = 1;
    Lexema *m_sym;
    InbufCurrentString *currstr;

    self->token.type = TOK_NONE;
    self->token.value = 0;
    self->token.len = 0;
    self->token.ident = 0;
    currstr = inbuf_currstr();

    for (;self->token.type == TOK_NONE;)
    {
        if (m_ch == CH_NULL)
            m_ch = inbuf_next_char();
        if (m_ch == eof_sym)
        {
            self->token.type = L_EOF;
            self->token.kind = SYM;
            self->token.ident = (char *)&eof_sym;
            continue;
        }

        m_sym = self->symbols;
        for ( ;m_sym->len != 0; )
        {
            if (*(m_sym->ident) == m_ch)
            {
                break;
            }
            m_sym++;
        }
        if (m_sym->type != TOK_NONE) // is symbol
        {
            self->token = *m_sym;
            continue;
        }

        if (is_decimal(m_ch)) // start with decimal digit returns number
        {
            char *ident = 0;
            int len = 0;
            char *endbuf;

            for (;is_digit(m_ch);)
            {
                ident = (char *)realloc(ident, len + 2);
                ident[len++] = m_ch;
                ident[len] = 0;
                m_ch = inbuf_next_char();
            }
            self->token.len = len;
            self->token.ident = 0;
            self->token.type = TOK_NUM;
            self->token.kind = CONST;
            self->token.value = strtol(ident, &endbuf, radix(m_ch));
            free(ident);
            continue;
        }
        if (is_alfa(m_ch)) // start with alfa returns keyword or ident
        {
            char *ident = 0;
            int len = 0;
            for ( ;is_alfa(m_ch) || is_digit(m_ch);)
            {
                // collect identifier
                ident = (char *)realloc(ident, len + 2);
                ident[len++] = m_ch;
                ident[len] = 0;
                m_ch = inbuf_next_char();
            }
            m_sym = self->words;
            for ( ;m_sym->len != 0;)
            {
                // compare with keywords
                if (!strcmp(ident, m_sym->ident))
                {
                    break;
                }
                m_sym++;
            }
            if (m_sym->type != TOK_NONE) // is keyword
            {
                self->token = *m_sym;
                free(ident);
                continue;
            }
            if (len < 255) // default is variable
            {
                self->token.kind = VAR;
                self->token.type = TOK_IDENT;
                self->token.ident = ident;
                self->token.len = len;
                continue;
            }
            throw_error(E_UNKIDENT, ident);
            exit_nicely(E_UNKIDENT);
        }
        fprintf(stderr, "In string: %d %s\n", currstr->num, currstr->str);
        throw_error(E_UNEXPSYM, &m_ch);
        self->token.kind = KIND_NONE;
        self->token.type = TOK_NONE;
        self->token.ident = 0;
        self->token.len = 0;
        break;
    }
    self->ch = m_ch;
    if (self->token.type == L_EOF)
        f_result = 0;

    return f_result;
}

static void lexer_print_tok(Lexema token)
{
    switch (token.type)
    {
    case L_EOF:
    case L_EOL:
        break;
    case TOK_NUM:
        printf("Token: %d \n", token.value);
        break;
    case TOK_IDENT:
    default:
        printf("Token: %s\n", token.ident);
    }
}

static void lexer_skip_one(Lexer *self)
{
    self->ch = inbuf_next_char();
}

static void lexer_skip_while(Lexer *self, unsigned char symbol)
{
    for ( ;self->ch == symbol && self->ch != 0xff;self->ch = inbuf_next_char());
}

static void lexer_skip_until(Lexer *self, unsigned char symbol)
{
    for (;self->ch != symbol && self->ch != 0xff; self->skipOne(self));
    self->nextTok(self);
}

int lexer_init(Lexer *lexer, const char *buf, int size)
{
    lexer->buf = buf;
    lexer->bufsize = size;
    lexer->words = (Lexema *)words;
    lexer->symbols = (Lexema *)symbols;
    lexer->ch = TOK_NONE;
    lexer->nextTok = lexer_next_tok;
    lexer->printTok = lexer_print_tok;
    lexer->skipWhile = lexer_skip_while;
    lexer->skipUntil = lexer_skip_until;
    lexer->skipOne = lexer_skip_one;
    lexer->token.type = TOK_NONE;
    lexer->token.ident = 0;
    inbuf_init(buf, size);
    return 1;
}

Lexer *lexer_create(const char *buf, int size)
{
    Lexer *m_lexer = (Lexer *)malloc(sizeof(Lexer));
    lexer_init(m_lexer, buf, size);
    return m_lexer;
}

void lexer_free(Lexer *self)
{
    free(self);
}
