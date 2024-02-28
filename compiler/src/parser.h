#ifndef H_PARSER_H
#define H_PARSER_H
#include <stdlib.h>
#include <lexer.h>

typedef struct _Parser{

    void (*parse)(pLexer m_lexer);

}Parser, *pParser;

pParser parser_create(void);
void parser_free(pParser _parser);

#endif
