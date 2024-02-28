#include <symbols.h>
#include <inbuf.h>
#include <lexer.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// imports
void exitNicely();
extern const char eof_sym;
extern const char eol_sym;

static const Lexema words[] =
    {
        {MOV, "MOV", 0, 3},
        {MVI, "MVI", 0, 3},
        {LXI, "LXI", 0, 3},
        {REGA, "A", 0, 1},
        {REGB, "B", 0, 1},
        {REGC, "C", 0, 1},
        {REGD, "D", 0, 1},
        {REGE, "E", 0, 1},
        {REGH, "H", 0, 1},
        {REGL, "L", 0, 1},
        {REGM, "M", 0, 1},
        {REGSP, "SP", 0, 1},
        {0, 0, 0, 0}};

static const Lexema symbols[] =
    {
        {COLON, ":", 0, 1},
        {SEMICOLON, ";", 0, 1},
        {COMMA, ",", 0, 1},
        {EQ, "=", 0, 1},
        {GT, ">", 0, 1},
        {LT, "<", 0, 1},
        {PLUS, "+", 0, 1},
        {MINUS, "-", 0, 1},
        {SPACE, " ", 0, 1},
        {L_EOL, (char *)&eol_sym, 0, 1},
        {L_EOF, (char *)&eof_sym, 0, 1},
        {0, 0, 0, 0}};

int lexer_next_tok(Lexer *lexer)
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
                lexer->token.type = ID;
                lexer->token.ident = ident;
                continue;
            }
            printf("Unknown identifier %s\n", ident);
            exitNicely();
        }
        printf("unexpected symbol %c\n", m_ch);
        exitNicely();
    }
    lexer->ch = m_ch;
    if (lexer->token.type == L_EOF)
        f_result = 0;

    return f_result;
}

pLexer lexer_create(int fd_in)
{
    pLexer m_lexer = (pLexer)malloc(sizeof(Lexer));
    m_lexer->fd_in = fd_in;
    m_lexer->words = (Lexema *)words;
    m_lexer->symbols = (Lexema *)symbols;
    m_lexer->ch = NONE;
    m_lexer->next_tok = lexer_next_tok;
    m_lexer->token.type = NONE;
    m_lexer->token.ident = 0;
    inbufInit(fd_in);
    return m_lexer;
}

void lexer_free(pLexer lexer)
{
    free(lexer);
}

void lexerPrintToken(Lexema token)
{
    switch (token.type)
    {
    case L_EOF:
    case L_EOL:
        break;
    case NUM:
        printf("Token: %d \n", token.value);
        break;
    case ID:
    default:
        printf("Token: %s\n", token.ident);
    }
}
