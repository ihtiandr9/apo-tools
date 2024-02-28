#include <errors.h>
#include <fcntl.h>
#include <stdlib.h>

extern int fd_in;
extern int fd_out;

void exitNicely(eErrLvl errorlevel)
{
    close(fd_in);
    close(fd_out);
    exit(errorlevel);
}

