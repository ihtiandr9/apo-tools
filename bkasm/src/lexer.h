#ifndef H_LEXER_H
#define H_LEXER_H

#include <globals.h>
#define CH_NULL 0
typedef enum _eIdentKind
{
  KIND_NONE = 0,
  OP,
  REG,
  SYM,
  CONST
} eIdentKind;

typedef enum _eIdentType
{
  TOK_NONE = 0,
  TOK_MOV,
  TOK_MVI,
  TOK_LXI,
  TOK_JMP,
  TOK_COLON,
  TOK_SEMICOLON,
  TOK_COMMA,
  TOK_EQ,
  TOK_GT,
  TOK_LT,
  TOK_NUM,
  TOK_IDENT,
  TOK_REGA,
  TOK_REGB,
  TOK_REGC,
  TOK_REGD,
  TOK_REGE,
  TOK_REGH,
  TOK_REGL,
  TOK_REGM,
  TOK_REGSP,
  TOK_PLUS,
  TOK_MINUS,
  TOK_ASTERISK,
  L_EOL,
  L_EOF
} eIdentType;

STRUCT(Lexema)
{
  eIdentKind kind;
  eIdentType type;
  char *ident;
  int value;
  int len;
};

STRUCT(Lexer)
{
  unsigned char ch;
  FILE *in_file;
  Lexema token;
  Lexema *symbols;
  Lexema *words;
  int (*nextTok)(Lexer *self);
  void (*printTok)(Lexema token);
  void (*skipUntil)(Lexer *self, unsigned char symbol);
  void (*skipWhile)(Lexer *self, unsigned char symbol);
  void (*skipOne)(Lexer *self);
};

// exports
Lexer *lexer_create(FILE *in_file);
int lexer_init(Lexer *lexer, FILE *in_file);
void lexer_free(Lexer *self);
#endif
