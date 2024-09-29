#define MAX_LABEL_SIZE 50
#define STRUCT(XXX) typedef struct XXX XXX; \
	struct XXX
STRUCT(CFG_Tree);
STRUCT(Program);

extern int fd_in;
extern int fd_out;
void exit_nicely(void);
