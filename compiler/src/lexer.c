#include <globals.h>
#include <errors.h>
#include <symbols.h>
#include <inbuf.h>
#include <lexer.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// imports
extern const char eof_sym;
extern const char eol_sym;

static const Lexema words[] =
    {
        {OP, MOV, "MOV", 0, 3},
        {OP, MVI, "MVI", 0, 3},
        {OP, LXI, "LXI", 0, 3},
        {REG, REGA, "A", 0, 1},
        {REG, REGB, "B", 0, 1},
        {REG, REGC, "C", 0, 1},
        {REG, REGD, "D", 0, 1},
        {REG, REGE, "E", 0, 1},
        {REG, REGH, "H", 0, 1},
        {REG, REGL, "L", 0, 1},
        {REG, REGM, "M", 0, 1},
        {REG, REGSP, "SP", 0, 1},
        {0, 0, 0, 0, 0}};

static const Lexema symbols[] =
    {
        {SYM, COLON, ":", 0, 1},
        {SYM, SEMICOLON, ";", 0, 1},
        {SYM, COMMA, ",", 0, 1},
        {SYM, EQ, "=", 0, 1},
        {SYM, GT, ">", 0, 1},
        {SYM, LT, "<", 0, 1},
        {SYM, PLUS, "+", 0, 1},
        {SYM, MINUS, "-", 0, 1},
        {SYM, SPACE, " ", 0, 1},
        {SYM, L_EOL, (char *)&eol_sym, 0, 1},
        {SYM, L_EOF, (char *)&eof_sym, 0, 1},
        {0, 0, 0, 0, 0}};

static int lexer_next_tok(Lexer *self)
{
    char m_ch = self->ch;
    int f_result = 1;
    self->token.type = NONE;
    self->token.value = 0;
    self->token.len = 0;
    self->token.ident = 0;

    while (self->token.type == NONE)
    {
        if (m_ch == NONE)
            m_ch = inbuf_next_char();
        else
            self->ch = NONE;
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
        if (m_sym->type != NONE) // is symbol
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
            self->token.type = NUM;
            self->token.kind = VAR;
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
            if (m_sym->type != NONE) // is keyword
            {
                self->token = *m_sym;
                free(ident);
                continue;
            }
            if (len < 255) // default is ident
            {
                self->token.kind = VAR;
                self->token.type = IDENT;
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
    case SPACE:
        break;
    case NUM:
        printf("Token: %d \n", token.value);
        break;
    case IDENT:
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
        self->ch = NONE;
        self->nextTok(self);
    }
    self->ch = NONE;
}

pLexer lexer_create(int fd_in)
{
    pLexer m_lexer = (pLexer)malloc(sizeof(Lexer));
    m_lexer->fd_in = fd_in;
    m_lexer->words = (Lexema *)words;
    m_lexer->symbols = (Lexema *)symbols;
    m_lexer->ch = NONE;
    m_lexer->nextTok = lexer_next_tok;
    m_lexer->printTok = lexer_print_tok;
    m_lexer->skipWhile = lexer_skip_while;
    m_lexer->skipUntil = lexer_skip_until;
    m_lexer->token.type = NONE;
    m_lexer->token.ident = 0;
    inbuf_init(fd_in);
    return m_lexer;
}

void lexer_free(pLexer self)
{
    free(self);
}
