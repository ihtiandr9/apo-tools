
#include <globals.h>
#include <unistd.h>
#include <stdlib.h>

int fd_in;
int fd_out;

void exit_nicely(void)
{
    close(fd_in);
    close(fd_out);
    exit(-1);
}
