#ifndef H_LEXER_H
#define H_LEXER_H

#define CH_NULL 0

typedef enum _eIdentKind
{
    KIND_NONE = 0,
    OP,
    REG,
    SYM,
    CONST,
    VAR,
    INT,
} eIdentKind;

typedef enum _eIdentType
{
    TOK_NONE = 0,

    // Mnemonics
    TOK_ACI,
    TOK_ADC,
    TOK_ADD,
    TOK_ADI,
    TOK_ANA,
    TOK_AND,
    TOK_ANI,
    TOK_CALL,
    TOK_CC,
    TOK_CM,
    TOK_CMA,
    TOK_CMC,
    TOK_CMP,
    TOK_CNC,
    TOK_CNZ,
    TOK_CP,
    TOK_CPE,
    TOK_CPI,
    TOK_CPO,
    TOK_CZ,
    TOK_DAA,
    TOK_DAD,
    TOK_DB,
    TOK_DCR,
    TOK_DCX,
    TOK_DI,
    TOK_EI,
    TOK_END,
    TOK_EQU,
    TOK_HLT,
    TOK_IN,
    TOK_INR,
    TOK_INX,
    TOK_JC,
    TOK_JM,
    TOK_JMP,
    TOK_JNC,
    TOK_JNZ,
    TOK_JP,
    TOK_JPE,
    TOK_JPO,
    TOK_JZ,
    TOK_LDA,
    TOK_LDAX,
    TOK_LHLD,
    TOK_LXI,
    TOK_MOV,
    TOK_MVI,
    TOK_NOP,
    TOK_ORA,
    TOK_ORG,
    TOK_ORI,
    TOK_OUT,
    TOK_PCHL,
    TOK_POP,
    TOK_PUSH,
    TOK_RAL,
    TOK_RAR,
    TOK_RC,
    TOK_RET,
    TOK_RIM,
    TOK_RLC,
    TOK_RM,
    TOK_RNC,
    TOK_RNZ,
    TOK_RP,
    TOK_RPE,
    TOK_RPO,
    TOK_RRC,
    TOK_RST,
    TOK_RZ,
    TOK_SBB,
    TOK_SBI,
    TOK_SHLD,
    TOK_SIM,
    TOK_SPHL,
    TOK_STA,
    TOK_STAX,
    TOK_STC,
    TOK_SUB,
    TOK_SUI,
    TOK_XCHG,
    TOK_XRA,
    TOK_XRI,
    TOK_XTHL,

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
    L_EOF,
    L_EOL,
    TOK_ASTERISK,
    TOK_COLON,
    TOK_COMMA,
    TOK_EQ,
    TOK_GT,
    TOK_IDENT,
    TOK_LT,
    TOK_MINUS,
    TOK_NUM,
    TOK_PLUS,
    TOK_SEMICOLON
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
    const char *buf;
    int bufsize;
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
Lexer *lexer_create(const char *buf, int size);
int lexer_init(Lexer *lexer, const char *buf, int size);
void lexer_free(Lexer *self);
#endif
