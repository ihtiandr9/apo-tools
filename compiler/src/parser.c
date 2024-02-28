#include <globals.h>
#include <errors.h>
#include <symbols.h>
#include <inbuf.h>
#include <lexer.h>
#include <parser.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

static void parser_parse(pLexer m_lexer)
{
    while (m_lexer->next_tok(m_lexer))
    {
        Lexema m_token = m_lexer->token;
        m_lexer->print_tok(m_token);
        switch (m_token.kind)
        {
        case SYM:
            switch (m_token.type)
            {
            case SEMICOLON:
                printf("    < COMMENT >: skip until eol\n");
                m_lexer->skip_until(m_lexer, 10);
                break;
            case IDENT:;
                char *m_ident = m_token.ident;
                m_lexer->next_tok(m_lexer);
                m_token = m_lexer->token;
                if (m_token.type == COLON)
                {
                    printf("    < LABEL >: %s\n", m_ident);
                    m_lexer->ch = NONE;
                }
                else
                {
                    throw_error(E_UNKIDENT, m_ident);
                    exitNicely();
                }
                break;
            case L_EOL: case COMMA: 
                m_lexer->ch = NONE;
                break;                
            case SPACE:
                m_lexer->skip_while(m_lexer, ' ');
                break;
            default:
                throw_error(E_UNEXPSYM, m_token.ident);
            }
            break;
        case OP:
            printf("    < OPERATION >: %s\n", m_token.ident);
            m_lexer->ch=NONE;
            break;
        case REG:
            printf("    < REGISTER >: %s\n", m_token.ident);
            m_lexer->ch=NONE;
            break;

        case VARIABLE:
            switch (m_token.type)
            {
            default:
                printf("    < VALUE >: %d\n", m_token.value);
            }
            break;
        default:
            throw_error(E_UNEXPTOKEN, m_token.ident);
            break;
        }
    }
}

static void parse_line(pLexer m_lexer)
{
}

pParser parser_create()
{
    pParser m_parser = (pParser)malloc(sizeof(Parser));
    m_parser->parse = parser_parse;
    return m_parser;
}

void parser_free(pParser _parser)
{
    free(_parser);
}