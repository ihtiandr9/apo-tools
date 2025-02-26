#ifndef H_GLOBALS_H
#define H_GLOBALS_H

#define INDENT "    "
#define MAX_LABEL_SIZE 50
#define STRUCT(XXX)         \
	typedef struct XXX XXX; \
	struct XXX

STRUCT(NodeList);
STRUCT(Gfg_Tree);

#include <stdio.h>
extern FILE *in_file;
extern FILE *out_file;
void exit_nicely(int);
#endif
