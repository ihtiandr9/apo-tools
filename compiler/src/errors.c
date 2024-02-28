#include <globals.h>

void exitNicely()
{
    close(fd_in);
    close(fd_out);
    lexer_free(m_lexer);
    exit(-1);
}
