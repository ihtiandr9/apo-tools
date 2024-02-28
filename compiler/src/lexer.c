
#include <stdio.h>
#include <globals.h>
#include <errors.h>
#include <string.h>
#include <stdlib.h>
#include <symbols.h>
#include <inbuf.h>
#include <lexer.h>

// imports
extern const char eof_sym;
extern const char eol_sym;

static const Lexema words[] =
    {
        {OP, TOK_MOV, "MOV", 0, 3},
        {OP, TOK_MVI, "MVI", 0, 3},
        {OP, TOK_LXI, "LXI", 0, 3},
        {REG, TOK_REGA, "A", 0, 1},
        {REG, TOK_REGB, "B", 0, 1},
        {REG, TOK_REGC, "C", 0, 1},
        {REG, TOK_REGD, "D", 0, 1},
        {REG, TOK_REGE, "E", 0, 1},
        {REG, TOK_REGH, "H", 0, 1},
        {REG, TOK_REGL, "L", 0, 1},
        {REG, TOK_REGM, "M", 0, 1},
        {REG, TOK_REGSP, "SP", 0, 1},
        {0, 0, 0, 0, 0}};

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
        {SYM, TOK_SPACE, " ", 0, 1},
        {SYM, L_EOL, (char *)&eol_sym, 0, 1},
        {SYM, L_EOF, (char *)&eof_sym, 0, 1},
        {0, 0, 0, 0, 0}};

static int lexer_next_tok(Lexer *self)
{
    char m_ch = self->ch;
    int f_result = 1;
    self->token.type = TOK_NONE;
    self->token.value = 0;
    self->token.len = 0;
    self->token.ident = 0;

    while (self->token.type == TOK_NONE)
    {
        if (m_ch == TOK_NONE)
            m_ch = inbuf_next_char();
        else
            self->ch = TOK_NONE;
        if (m_ch == eof_sym)
        {
            self->token.type = L_EOF;
            self->token.kind = SYM;
            self->token.ident = (char *)&eof_sym;
            continue;
        }

        Lexema *m_sym = self->symbols;
        while (m_sym->len != 0)
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

        if (is_digit(m_ch)) // is digit returns number
        {
            char *ident = 0;
            int len = 0;
            int value = 0;
            while (is_digit(m_ch))
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
            self->token.value = atoi(ident);
            free(ident);
            continue;
        }
        if (is_alfa(m_ch)) // try read keyword or ident
        {
            char *ident = 0;
            int len = 0;
            while (is_alfa(m_ch))
            {
                ident = (char *)realloc(ident, len + 2);
                ident[len++] = m_ch;
                ident[len] = 0;
                m_ch = inbuf_next_char();
            }
            m_sym = self->words;
            while (m_sym->len != 0)
            {
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
                continue;
            }
            throw_error(E_UNKIDENT, ident);
            exit_nicely();
        }
        throw_error(E_UNEXPSYM, &m_ch);
        exit_nicely();
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
    case TOK_SPACE:
        break;
    case TOK_NUM:
        printf("Token: %d \n", token.value);
        break;
    case TOK_IDENT:
    default:
        printf("Token: %s\n", token.ident);
    }
}

static void lexer_skip_while(Lexer *self, unsigned char symbol)
{
    while (self->ch == symbol && self->ch != 0xff)
    {
        self->ch = inbuf_next_char();
    }
}

static void lexer_skip_until(Lexer *self, unsigned char symbol)
{
    while (self->ch != symbol && self->ch != 0xff)
    {
        self->ch = TOK_NONE;
        self->nextTok(self);
    }
    self->ch = TOK_NONE;
}

pLexer lexer_create(int fd_in)
{
    pLexer m_lexer = (pLexer)malloc(sizeof(Lexer));
    m_lexer->fd_in = fd_in;
    m_lexer->words = (Lexema *)words;
    m_lexer->symbols = (Lexema *)symbols;
    m_lexer->ch = TOK_NONE;
    m_lexer->nextTok = lexer_next_tok;
    m_lexer->printTok = lexer_print_tok;
    m_lexer->skipWhile = lexer_skip_while;
    m_lexer->skipUntil = lexer_skip_until;
    m_lexer->token.type = TOK_NONE;
    m_lexer->token.ident = 0;
    inbuf_init(fd_in);
    return m_lexer;
}

void lexer_free(pLexer self)
{
    free(self);
}
