#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <globals.h>
#include <lexer.h>
#include <parser.h>
#include <cfg_tree.h>

static const int default_fd_in = 1;
static const int default_fd_out = 1;

int main(int argc, char *argv[])
{

    fd_in = default_fd_in;
    fd_out = default_fd_out;
	struct Lexer m_lexer;
	struct Parser m_parser;
	struct Program program;
    program.first = 0;
    program.last = 0;

    if (argc > 1)
    {
        fd_in = open(argv[1], O_RDONLY);
    }
    if (argc > 2)
    {
        fd_out = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 511);
    }

    if (fd_in < 0)
    {
        printf("file not exist");
        exit_nicely();
    }
    if (fd_in < 0)
    {
        printf("file not create");
        exit_nicely();
    }

    lexer_init(&m_lexer, fd_in);
    parser_init(&m_parser);
    m_parser.parse(&m_parser, &m_lexer, &program);

    close(fd_in);
    close(fd_out);
    destroyProgram(&program);
    return 0;
}
