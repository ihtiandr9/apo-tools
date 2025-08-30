#ifndef H_BKASM_H
#define H_BKASM_H

#define INDENT "    "
#define MAX_LABEL_SIZE 50
#define STRUCT(XXX)         \
	typedef struct XXX XXX; \
	struct XXX
#define MAX_PROG_SIZE 4096
#define VAR_COUNT 256

STRUCT(NodeList);
STRUCT(GfgTree);

typedef enum{ PARSE_STAGE = 0,
	EVAL_STAGE,
	GENERATE_STAGE,
} BkasmStage;

typedef struct _IO_FILE FILE;
extern FILE *infile;
extern FILE *outfile;
extern BkasmStage bkasm_stage;
void exit_nicely(int);

#endif
