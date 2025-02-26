
#include <bkasm.h>
#ifndef WIN32
#ifndef _Windows
#ifndef __MSDOS__
#include <unistd.h>
#endif
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <cfg_tree.h>

FILE* in_file;
FILE* out_file;

void exit_nicely(int errorlevel)
{
    fclose(in_file);
    fclose(out_file);
    exit(errorlevel);
}
