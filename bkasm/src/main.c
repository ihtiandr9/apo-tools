#ifndef WIN32
#include <unistd.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <globals.h>
#include <lexer.h>
#include <parser.h>
#include <cfg_tree.h>

extern Program program;

int main(int argc, char *argv[])
{
    Lexer m_lexer;
    Parser m_parser;

    in_file = stdin;
    out_file = stdout;

    if (argc > 1)
    {
        in_file = fopen(argv[1], "r");
    }

    if (argc > 2)
    {
        out_file = fopen(argv[2], "w");
    }

    if (!in_file)
    {
        printf("file not exist");
        exit_nicely(-1);
    }
    if (!out_file)
    {
        printf("file not create");
        exit_nicely(-1);
    }

    lexer_init(&m_lexer, in_file);
    parser_init(&m_parser);
    program_init(&program);
    parser_parse(&m_parser, &m_lexer, &program);

    exit_nicely(0);
    return 0;
}
