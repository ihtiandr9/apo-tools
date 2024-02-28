#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <globals.h>
#include <lexer.h>
#include <parser.h>

static const int default_fd_in = 1;
static const int default_fd_out = 1;

int main(int argc, char *argv[])
{

    fd_in = default_fd_in;
    fd_out = default_fd_out;

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
    Lexer *m_lexer = lexer_create(fd_in);
    Parser *m_parser = parser_create();
    if (m_parser)
        m_parser->parse(m_parser, m_lexer);

    parser_free(m_parser);
    lexer_free(m_lexer);
    close(fd_in);
    close(fd_out);
    return 0;
}
