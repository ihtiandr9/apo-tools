#ifndef H_GLOBALS_H
#define H_GLOBALS_H

#define INDENT "    "
#define MAX_LABEL_SIZE 50
#define STRUCT(XXX) typedef struct XXX XXX; \
	struct XXX
STRUCT(CFG_Tree);
STRUCT(Program);

extern int fd_in;
extern int fd_out;
void exit_nicely(int);
#endif

