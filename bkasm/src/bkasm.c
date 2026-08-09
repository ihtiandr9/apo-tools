
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
#include "bkasm.h"
#include "asmast.h"
#include "inbuf.h"

FILE* infile;
FILE* outfile;
BkasmStage bkasm_stage;

void exit_nicely(int errorlevel)
{
    inbuf_cleanup();
    if (infile) fclose(infile);
    if (outfile) fclose(outfile);
    exit(errorlevel);
}
