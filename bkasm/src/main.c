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
#include "bkasm.h"
#include "asmast.h"
#include "codegen.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    Lexer m_lexer;
    Parser m_parser;
    int bufsize;
    static char buf[MAX_PROG_SIZE];

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

    bufsize = fread(buf, 1, MAX_PROG_SIZE, infile);
    printf("program text size=%d\n", bufsize);
    lexer_init(&m_lexer, buf, bufsize);
    parser_init(&m_parser);
    parser_parse(&m_parser, &m_lexer);
    codegen_link(m_parser.ast);

    ast_destroy(m_parser.ast);
    exit_nicely(0);
    return 0;
}
