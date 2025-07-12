#ifndef H_GLOBALS_H
#define H_GLOBALS_H

#define INDENT "    "
#define MAX_LABEL_SIZE 50
#define STRUCT(XXX)         \
	typedef struct XXX XXX; \
	struct XXX
#define MAX_PROG_SIZE 4096

STRUCT(NodeList);
STRUCT(GfgTree);

#include <stdio.h>
extern FILE *infile;
extern FILE *outfile;
void exit_nicely(int);
#endif
