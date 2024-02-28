#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lexer.h>

static int fd_in;
static int fd_out;
static const int default_fd_in=1;
static const int default_fd_out=1;

void exitNicely()
{
    close(fd_in);
    close(fd_out);
    exit(-1);
}

int main(int argc, char *argv[])
{

    fd_in = default_fd_in;
    fd_out = default_fd_out;

    if(argc > 1)
    {
        fd_in = open(argv[1], O_RDONLY);
    }
    if(argc > 2)
    {
     fd_out = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 511);
    }

    if (fd_in < 0)
    {
        printf("file not exist");
        exitNicely();
    }
    if (fd_in < 0)
    {
        printf("file not create");
        exitNicely();
    }
//    Lexer *m_lexer = lexer_create(fd_in);
//    m_lexer->next_tok(m_lexer);
    close(fd_in);
    close(fd_out);
    return 0;
}
