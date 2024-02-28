
typedef enum
{
  MOV,
  MVI,
  LXI
} eIdentType;

typedef struct t_Lexema
{
  eIdentType type;
  char       ident;
} Lexema;

typedef struct t_lexer
{
    Lexema *symbols;
    Lexema *words;
} Lexer;
