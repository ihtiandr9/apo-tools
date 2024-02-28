#include <lexer.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// imports
void exitNicely();

// statics
static const char lexer_eof = 0xFF;
static const char lexer_eol = 0x0A;

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
        {COMMA, ",", 0, 1},
        {EQ, "=" , 0, 1},
        {GT, ">" , 0, 1},
        {LT, "<" , 0, 1},
        {SPACE, " ", 0, 1},
        {L_EOL, (char *)&lexer_eol, 0, 1},
        {L_EOF, (char *)&lexer_eof, 0, 1},
        {0, 0, 0, 0}};
int isDigit(char ch)
{
    const char digit[] = "0123456789";
    int i = 0;
    for (i = 0; i < sizeof(digit) - 1; i++)
    {
        if (ch == digit[i])
            break;
    }
    return (ch == digit[i]);
}

int isAlfa(char ch)
{
    const char alfa[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i = 0;
    // printf("Size of alfa: %d ", sizeof(alfa));
    for (i = 0; i < sizeof(alfa) - 1; i++)
    {
        if (ch == alfa[i])
            break;
    }
    return (ch == alfa[i]);
}

int lexer_next_tok(Lexer *lexer)
{
    char m_ch = lexer->ch;
    int readed;

    lexer->token.type = NONE;
    lexer->token.value = 0;
    lexer->token.len = 0;
    lexer->token.ident = 0;
    while (lexer->token.type == NONE)
    {
        if (m_ch == NONE)
        {
            readed = read(lexer->fd_in, &m_ch, 1);
            if (!readed)
            {
                m_ch = lexer_eof;
                lexer->token.type = L_EOF;
            }
        }
        else
        {
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
            if (isDigit(m_ch))
            {
                char *ident = 0;
                int len = 0;
                int value = 0;
                while (isDigit(m_ch))
                {
                    ident = (char *)realloc(ident, len + 2);
                    ident[len] = m_ch;
                    ident[len + 1] = 0;
                    len++;
                    readed = read(lexer->fd_in, &m_ch, 1);
                }
                lexer->token.len = len;
                lexer->token.ident = 0;
                lexer->token.type = NUM;
                lexer->token.value = atoi(ident);
                continue;
            }
            if (isAlfa(m_ch))
            {
                char *ident = 0;
                int len = 0;
                while (isAlfa(m_ch))
                {
                    ident = (char *)realloc(ident, len + 2);
                    ident[len] = m_ch;
                    ident[len + 1] = 0;
                    len++;
                    readed = read(lexer->fd_in, &m_ch, 1);
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
                if (m_sym->type != NONE) // is WORD
                {
                    lexer->token = *m_sym;
                    free(ident);
                    continue;
                }
                if (len < 10)
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
    }
    lexer->ch = m_ch;

    return 1;
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
    return m_lexer;
}

void lexer_free(pLexer lexer)
{
    free(lexer);
}