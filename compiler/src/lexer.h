#ifndef H_LEXER_H
#define H_LEXER_H
#include <stdlib.h>

typedef enum _eIdentKind
{
  KIND_NONE = 0,
  OP,
  REG,
  SYM,
  VAR
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

typedef struct _Lexema
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
  int (*nextTok)(struct t_lexer *self);
  void (*printTok)(Lexema token);
  void (*skipUntil)(struct t_lexer *self, unsigned char symbol);
  void (*skipWhile)(struct t_lexer *self, unsigned char symbol);
} Lexer, *pLexer;

// exports
pLexer lexer_create(int fd_in);
void lexer_free(pLexer self);
#endif
