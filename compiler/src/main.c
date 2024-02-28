#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <globals.h>

// imports
void exitNicely();

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
        exitNicely();
    }
    if (fd_in < 0)
    {
        printf("file not create");
        exitNicely();
    }
    m_lexer = lexer_create(fd_in);

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

    lexer_free(m_lexer);
    m_lexer = 0;
    close(fd_in);
    close(fd_out);
    return 0;
}
