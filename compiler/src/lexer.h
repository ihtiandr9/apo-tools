#ifndef H_LEXER_H
#define H_LEXER_H
#include <stdlib.h>

typedef enum
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
  ID,
  REGA,
  REGB,
  REGC,
  REGD,
  REGE,
  REGH,
  REGL,
  REGM,
  REGSP,
  E_UNEXP,
  E_UNKID,
  L_EOL,
  L_EOF
} eIdentType;

typedef struct t_Lexema
{
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
} Lexer, *pLexer;

// exports
void lexerPrintToken(Lexema token);
pLexer lexer_create(int fd_in);
void lexer_free(pLexer lexer);
#endif
