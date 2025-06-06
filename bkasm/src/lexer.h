#ifndef H_LEXER_H
#define H_LEXER_H

#include <bkasm.h>
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

    // Mnemonics
    TOK_ANI,
    TOK_CALL,
    TOK_COLON,
    TOK_DB,
    TOK_DCR,
    TOK_DCX,
    TOK_END,
    TOK_EQU,
    TOK_EI,
    TOK_INR,
    TOK_INX,
    TOK_JMP,
    TOK_JZ,
    TOK_LHLD,
    TOK_LXI,
    TOK_MOV,
    TOK_MVI,
    TOK_ORG,
    TOK_RET,
    TOK_XCHG,

    // Registers
    TOK_REGA,
    TOK_REGB,
    TOK_REGC,
    TOK_REGD,
    TOK_REGE,
    TOK_REGH,
    TOK_REGL,
    TOK_REGM,
    TOK_REGBC,
    TOK_REGDE,
    TOK_REGHL,
    TOK_REGPC,
    TOK_REGSP,

    // Symbols
    TOK_SEMICOLON,
    TOK_COMMA,
    TOK_EQ,
    TOK_GT,
    TOK_LT,
    TOK_NUM,
    TOK_IDENT,
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
Lexer *lexer_create(FILE *infile);
int lexer_init(Lexer *lexer, FILE *infile);
void lexer_free(Lexer *self);
#endif
