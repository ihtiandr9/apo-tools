
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
#include <asmast.h>

FILE* infile;
FILE* outfile;

void exit_nicely(int errorlevel)
{
    fclose(infile);
    fclose(outfile);
    exit(errorlevel);
}
