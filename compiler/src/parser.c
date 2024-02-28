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
        if (m_lexer->token.type == L_EOF)
            break;
        if (m_lexer->token.type == SPACE 
	    || m_lexer->token.type == DOTCOMMA 
	    || m_lexer->token.type == COMMA 
	    || m_lexer->token.type == L_EOL)
        {
            m_lexer->ch = NONE;
        }
        lexerPrintToken(m_lexer->token);
    }
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