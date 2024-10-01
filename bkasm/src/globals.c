
#include <globals.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <stdlib.h>
#include <cfg_tree.h>

int fd_in;
int fd_out;
Program program;

void exit_nicely(int errorlevel)
{
    close(fd_in);
    close(fd_out);
    program_destroy(&program);
    exit(errorlevel);
}
