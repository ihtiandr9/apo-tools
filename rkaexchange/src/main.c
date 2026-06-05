#include <edb.h>
#include <errors.h>
#include <fcntl.h>
#ifndef WIN32
#ifndef _Windows
#ifndef __MSDOS__
#include <unistd.h>
#endif
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <globals.h>
#include <assert.h>

#define CMD_PACK 1
#define CMD_UNPACK 2

int main(int argc, char *argv[])
{
    Edb edb;
	int cmd;
    /* initialize defaults fd_in fd_out and cmd
     * in future it will be read from command line
     */
    fd_in = stdin;
    fd_out = stdout;
	cmd = CMD_UNPACK;
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
        fd_in = fopen(argv[2], "r");
    }
    if (argc > 3)
    {
        fd_out = fopen(argv[3], "w");
    }

    if (fd_in < 0)
    {
        throw_error(E_NOEXIST, argv[2]);
    }
    if (fd_out < 0)
    {
        throw_error(E_CREATE, argv[3]);
    }
    edb_init(fd_in, fd_out, &edb);

    if (CMD_UNPACK == cmd)
        edb.unpack(&edb);
    else if (CMD_PACK == cmd)
        edb.pack(&edb);

    exit_nicely(0);
    return 0;
}
