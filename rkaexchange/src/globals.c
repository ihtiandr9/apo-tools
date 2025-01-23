
#include <globals.h>
#ifndef WIN32
#ifndef _Windows
#ifndef __MSDOS__
#include <unistd.h>
#endif
#endif
#endif
#include <stdlib.h>

int fd_in;
int fd_out;

void exit_nicely()
{
    close(fd_in);
    close(fd_out);
    exit(-1);
}