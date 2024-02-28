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

static int lexer_next_tok(Lexer *lexer)
{
    char m_ch = lexer->ch;
    int f_result = 1;
    lexer->token.type = NONE;
    lexer->token.value = 0;
    lexer->token.len = 0;
    lexer->token.ident = 0;

    while (lexer->token.type == NONE)
    {
        if (m_ch == NONE)
            m_ch = inbufNextChar();
        else
            lexer->ch = NONE;
        if (m_ch == eof_sym)
        {
            lexer->token.type = L_EOF;
            lexer->token.ident = (char *)&eof_sym;
            continue;
        }

        Lexema *m_sym = lexer->symbols;
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
            lexer->token = *m_sym;
            continue;
        }

        if (isDigit(m_ch)) // is digit returns number
        {
            char *ident = 0;
            int len = 0;
            int value = 0;
            while (isDigit(m_ch))
            {
                ident = (char *)realloc(ident, len + 2);
                ident[len++] = m_ch;
                ident[len] = 0;
                m_ch = inbufNextChar();
            }
            lexer->token.len = len;
            lexer->token.ident = 0;
            lexer->token.type = NUM;
            lexer->token.kind = VARIABLE;
            lexer->token.value = atoi(ident);
            free(ident);
            continue;
        }
        if (isAlfa(m_ch)) // try read keyword or ident
        {
            char *ident = 0;
            int len = 0;
            while (isAlfa(m_ch))
            {
                ident = (char *)realloc(ident, len + 2);
                ident[len++] = m_ch;
                ident[len] = 0;
                m_ch = inbufNextChar();
            }
            m_sym = lexer->words;
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
                lexer->token = *m_sym;
                free(ident);
                continue;
            }
            if (len < 255) // default is ident
            {
                lexer->token.type = IDENT;
                lexer->token.ident = ident;
                continue;
            }
            throw_error(E_UNKIDENT, ident);
            exitNicely();
        }
        throw_error(E_UNEXPSYM, &m_ch);
        exitNicely();
    }
    lexer->ch = m_ch;
    if (lexer->token.type == L_EOF)
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
    case NUM:
        printf("Token: %d \n", token.value);
        break;
    case IDENT:
    default:
        printf("Token: %s\n", token.ident);
    }
}

static void lexer_skip_while(Lexer *lexer, unsigned char symbol)
{
    while (lexer->ch == symbol && lexer->ch != 0xff)
    {
        lexer->ch = inbufNextChar();
    }
}

static void lexer_skip_until(Lexer *lexer, unsigned char symbol)
{
    while (lexer->ch != symbol && lexer->ch != 0xff)
    {
        lexer->ch = NONE;
        lexer->next_tok(lexer);
    }
}

pLexer lexer_create(int fd_in)
{
    pLexer m_lexer = (pLexer)malloc(sizeof(Lexer));
    m_lexer->fd_in = fd_in;
    m_lexer->words = (Lexema *)words;
    m_lexer->symbols = (Lexema *)symbols;
    m_lexer->ch = NONE;
    m_lexer->next_tok = lexer_next_tok;
    m_lexer->print_tok = lexer_print_tok;
    m_lexer->skip_while = lexer_skip_while;
    m_lexer->skip_until = lexer_skip_until;
    m_lexer->token.type = NONE;
    m_lexer->token.ident = 0;
    inbufInit(fd_in);
    return m_lexer;
}

void lexer_free(pLexer lexer)
{
    free(lexer);
}
