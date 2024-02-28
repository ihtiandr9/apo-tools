#ifndef H_LEXER_H
#define H_LEXER_H

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
  TOK_COLON,
  TOK_SEMICOLON,
  TOK_COMMA,
  TOK_EQ,
  TOK_GT,
  TOK_LT,
  TOK_PLUS,
  TOK_MINUS,
  TOK_SPACE,
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
  unsigned char ch;
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
