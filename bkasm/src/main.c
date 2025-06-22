#ifndef WIN32
#ifndef _Windows
#ifndef __MSDOS__
#include <unistd.h>
#endif
#endif
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <bkasm.h>
#include <lexer.h>
#include <parser.h>

int main(int argc, char *argv[])
{
    Lexer m_lexer;
    Parser m_parser;

    infile = stdin;
    outfile = stdout;

    if (argc > 1)
    {
        infile = fopen(argv[1], "r");
    }

    if (argc > 2)
    {
        outfile = fopen(argv[2], "w");
    }

    if (!infile)
    {
        printf("file not exist");
        exit_nicely(-1);
    }
    if (!outfile)
    {
        printf("file not create");
        exit_nicely(-1);
    }

    lexer_init(&m_lexer, infile);
    parser_init(&m_parser);
    parser_parse(&m_parser, &m_lexer);

    ast_destroy(m_parser.prog);
    exit_nicely(0);
    return 0;
}
