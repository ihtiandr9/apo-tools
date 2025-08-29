
#include <stdio.h>
#include <globals.h>
#ifndef WIN32
#ifndef _Windows
#ifndef __MSDOS__
#include <unistd.h>
#endif
#endif
#endif
#include <stdlib.h>

FILE *fd_in;
FILE *fd_out;

void exit_nicely(int err)
{
    fclose(fd_in);
    fclose(fd_out);
    exit(err);
}
