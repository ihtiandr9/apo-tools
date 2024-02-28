#include <edb.h>
#include <errors.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define CMD_PACK 1
#define CMD_UNPACK 2

#define DEFAULT_FD_IN 1
#define DEFAULT_FD_OUT 1

int fd_in;
int fd_out;

int main(int argc, char *argv[])
{

    fd_in = DEFAULT_FD_IN;
    fd_out = DEFAULT_FD_OUT;
    int cmd = CMD_UNPACK;
    if (argc <= 1)
    {
        printf("error: command \"pack\" or \"unpack\" required\n");
        exitNicely(E_CMDREQ);
    }
    else
    {
        if (!strcmp("pack", argv[1]))
            cmd = CMD_PACK;
        else if (!strcmp("unpack", argv[1]))
            cmd = CMD_UNPACK;
        else
        {
            printf("error: unexpected command %s\n", argv[1]);
            exitNicely(E_UNKNOWNCMD);
        }
    }
    if (argc > 2)
    {
        fd_in = open(argv[2], O_RDONLY);
    }
    if (argc > 3)
    {
        fd_out = open(argv[3], O_CREAT | O_WRONLY | O_TRUNC, 511);
    }

    if (fd_in < 0)
    {
        printf("file not exist\n");
        exitNicely(E_NOEXIST);
    }
    if (fd_in < 0)
    {
        printf("file not create\n");
        exitNicely(E_CREATE);
    }
    Edb *pEdb = edb_create(fd_in, fd_out);
    if (CMD_UNPACK == cmd)
        pEdb->unpack(pEdb);
    else if (CMD_PACK == cmd)
        pEdb->pack(pEdb);

    // unpack(fd_in, fd_out);
    close(fd_in);
    close(fd_out);
    return 0;
}
