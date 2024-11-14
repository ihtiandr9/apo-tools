
#include <globals.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <cfg_tree.h>

FILE* in_file;
FILE* out_file;
Program program;

void exit_nicely(int errorlevel)
{
    fclose(in_file);
    fclose(out_file);
    program_destroy(&program);
    exit(errorlevel);
}
