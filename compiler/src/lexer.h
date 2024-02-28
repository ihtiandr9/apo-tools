#ifndef H_LEXER_H
#define H_LEXER_H
#include <stdlib.h>

typedef enum
{
  KIND_NONE = 0,
  OP,
  REG,
  SYM,
  VARIABLE
} eIdentKind;

typedef enum _eIdentType
{
  NONE = 0,
  MOV,
  MVI,
  LXI,
  COLON,
  SEMICOLON,
  COMMA,
  EQ,
  GT,
  LT,
  PLUS,
  MINUS,
  SPACE,
  NUM,
  IDENT,
  REGA,
  REGB,
  REGC,
  REGD,
  REGE,
  REGH,
  REGL,
  REGM,
  REGSP,
  L_EOL,
  L_EOF
} eIdentType;

typedef struct t_Lexema
{
  eIdentKind kind;
  eIdentType type;
  char *ident;
  int value;
  int len;
} Lexema;

typedef struct t_lexer
{
  int fd_in;
  Lexema *symbols;
  Lexema *words;
  char ch;
  Lexema token;
  int (*next_tok)(struct t_lexer *lexer);
  void (*print_tok)(Lexema token);
  void (*skip_until)(struct t_lexer *lexer, unsigned char symbol);
  void (*skip_while)(struct t_lexer *lexer, unsigned char symbol);
} Lexer, *pLexer;

// exports
pLexer lexer_create(int fd_in);
void lexer_free(pLexer lexer);
#endif
