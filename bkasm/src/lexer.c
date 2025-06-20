
#include <stdio.h>
#include <bkasm.h>
#include <errors.h>
#include <string.h>
#include <stdlib.h>
#include <symbols.h>
#include <inbuf.h>
#include <lexer.h>
#include <bkints.h>

// imports
extern const char eof_sym;
extern const char eol_sym;

static const Lexema words[] =
    {
        {OP, TOK_DB, "DB", 0, 2},
        {OP, TOK_EI, "EI", 0, 2},
        {OP, TOK_JZ, "JZ", 0, 2},
        {OP, TOK_ANI, "ANI", 0, 3},
        {OP, TOK_DCR, "DCR", 0, 3},
        {OP, TOK_DCX, "DCX", 0, 3},
        {OP, TOK_END, "END", 0, 3},
        {OP, TOK_EQU, "EQU", 0, 3},
        {OP, TOK_INR, "INR", 0, 3},
        {OP, TOK_INX, "INX", 0, 3},
        {OP, TOK_JMP, "JMP", 0, 3},
        {OP, TOK_LXI, "LXI", 0, 3},
        {OP, TOK_MOV, "MOV", 0, 3},
        {OP, TOK_MVI, "MVI", 0, 3},
        {OP, TOK_ORG, "ORG", 0, 3},
        {OP, TOK_RET, "RET", 0, 3},
        {OP, TOK_CALL, "CALL", 0, 4},
        {OP, TOK_LHLD, "LHLD", 0, 4},
        {OP, TOK_XCHG, "XCHG", 0, 4},
        {REG, TOK_REGA, "A", 0, 1},
        {REG, TOK_REGB, "B", 0, 1},
        {REG, TOK_REGC, "C", 0, 1},
        {REG, TOK_REGD, "D", 0, 1},
        {REG, TOK_REGE, "E", 0, 1},
        {REG, TOK_REGH, "H", 0, 1},
        {REG, TOK_REGL, "L", 0, 1},
        {REG, TOK_REGM, "M", 0, 1},
        {REG, TOK_REGSP, "SP", 0, 2},
        {REG, TOK_REGHL, "HL", 0, 2},
        {REG, TOK_REGBC, "BC", 0, 2},
        {REG, TOK_REGDE, "DE", 0, 2},
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
            if (len < 255) // default is ident
            {
                self->token.kind = CONST;
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

int lexer_init(Lexer *lexer, FILE *in_file)
{
    lexer->in_file = in_file;
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
    inbuf_init(in_file);
    return 1;
}

Lexer *lexer_create(FILE *infile)
{
    Lexer *m_lexer = (Lexer *)malloc(sizeof(Lexer));
    lexer_init(m_lexer, infile);
    return m_lexer;
}

void lexer_free(Lexer *self)
{
    free(self);
}
