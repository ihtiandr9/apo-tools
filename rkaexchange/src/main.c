#include <edb.h>
#include <errors.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <globals.h>
#include <assert.h>

#define CMD_PACK 1
#define CMD_UNPACK 2

#define DEFAULT_FD_IN 1
#define DEFAULT_FD_OUT 1

int main(int argc, char *argv[])
{

    fd_in = DEFAULT_FD_IN;
    fd_out = DEFAULT_FD_OUT;
    int cmd = CMD_UNPACK;
    if (argc <= 1)
    {
        throw_error(E_CMDREQ, 0);        
    }
    else
    {
        if (!strcmp("pack", argv[1]))
            cmd = CMD_PACK;
        else if (!strcmp("unpack", argv[1]))
            cmd = CMD_UNPACK;
        else
        {
            throw_error(E_UNKNOWNCMD, argv[1]);            
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
        throw_error(E_NOEXIST, argv[2]);        
    }
    if (fd_out < 0)
    {
        throw_error(E_CREATE, argv[3]);        
    }
    Edb *pEdb = edb_create(fd_in, fd_out);
    if (CMD_UNPACK == cmd)
        pEdb->unpack(pEdb);
    else if (CMD_PACK == cmd)
        pEdb->pack(pEdb);

    close(fd_in);
    close(fd_out);
    return 0;
}
