namespace Bkasm
{
    public static class Constants
    {
        public const int MAX_PROG_SIZE = 65536;
        public const int MAX_LABEL_SIZE = 50;
        public const int MAX_VAR_COUNT = 1024;
        public const int MAX_ERR_MSG_LEN = 150;
        public const int INBUF_STRING_SIZE = 512;
        public const string INDENT = "    ";
        public const byte EOF_SYM = 0xFF;
        public const byte EOL_SYM = 0x0A;
        public const char CH_NULL = '\0';
        public const byte SPACE = (byte)' ';
        public const byte COMMA = (byte)',';
    }

    public enum BkasmStage
    {
        PARSE_STAGE = 0,
        EVAL_STAGE,
        GENERATE_STAGE,
    }

    public enum eIdentKind
    {
        KIND_NONE = 0,
        OP,
        REG,
        SYM,
        CONST,
        VAR,
        INT,
        STRING,
    }

    public enum eIdentType
    {
        TOK_NONE = 0,
        TOK_ACI, TOK_ADC, TOK_ADD, TOK_ADI, TOK_ANA, TOK_AND, TOK_ANI,
        TOK_CALL, TOK_CC, TOK_CM, TOK_CMA, TOK_CMC, TOK_CMP, TOK_CNC,
        TOK_CNZ, TOK_CP, TOK_CPE, TOK_CPI, TOK_CPO, TOK_CZ,
        TOK_DAA, TOK_DAD, TOK_DB, TOK_DCR, TOK_DCX, TOK_DI, TOK_DS,
        TOK_DW, TOK_EI, TOK_END, TOK_EQU, TOK_HLT, TOK_IN, TOK_INCLUDE, TOK_INR,
        TOK_INX, TOK_JC, TOK_JM, TOK_JMP, TOK_JNC, TOK_JNZ, TOK_JP,
        TOK_JPE, TOK_JPO, TOK_JZ, TOK_LDA, TOK_LDAX, TOK_LHLD, TOK_LXI,
        TOK_MOV, TOK_MVI, TOK_NOP, TOK_ORA, TOK_ORG, TOK_ORI, TOK_OUT,
        TOK_PCHL, TOK_POP, TOK_PUSH, TOK_RAL, TOK_RAR, TOK_RC, TOK_RET,
        TOK_RIM, TOK_RLC, TOK_RM, TOK_RNC, TOK_RNZ, TOK_RP, TOK_RPE,
        TOK_RPO, TOK_RRC, TOK_RST, TOK_RZ, TOK_SBB, TOK_SBI, TOK_SHLD,
        TOK_SIM, TOK_SPHL, TOK_STA, TOK_STAX, TOK_STC, TOK_SUB, TOK_SUI,
        TOK_XCHG, TOK_XRA, TOK_XRI, TOK_XTHL,
        TOK_REGA, TOK_REGB, TOK_REGC, TOK_REGD, TOK_REGE, TOK_REGH,
        TOK_REGL, TOK_REGM, TOK_REGBC, TOK_REGDE, TOK_REGHL, TOK_REGPC,
        TOK_REGSP, TOK_PSW,
        L_EOF, L_EOL,
        TOK_EQ, TOK_GT, TOK_LT, TOK_SEMICOLON, TOK_COLON,
        TOK_COMMA, TOK_IDENT, TOK_MINUS, TOK_ASTERISK, TOK_NUM, TOK_PLUS,
        TOK_LPAREN, TOK_RPAREN,
    }

    public enum eExprType
    {
        EXPR_EMPTY,
        EXPR_CONST,
        EXPR_REG,
        EXPR_VAR,
        EXPR_MATH,
    }

    public enum eNodeType
    {
        NODE_EMPTY = 0,
        NODE_INSTRUCTION,
        NODE_VAR,
        NODE_PSEUDO,
    }

    public enum eErrorType
    {
        E_SUCCESS,
        E_UNKIDENT,
        E_UNEXPSYM,
        E_UNEXPTOKEN,
        E_UNKKEYWORD,
        E_SYNTAXERROR,
        E_LINKERERROR,
        E_INTERNALERROR,
    }

    public struct IntBase
    {
        public char suffix;
        public int radix;
        public IntBase(char s, int r) { suffix = s; radix = r; }
    }

    public enum opRegs
    {
        OP_REGB = 0x0,
        OP_REGC = 0x1,
        OP_REGD = 0x2,
        OP_REGE = 0x3,
        OP_REGH = 0x4,
        OP_REGL = 0x5,
        OP_REGM = 0x6,
        OP_REGA = 0x7,
    }

    public enum opRegPairs
    {
        OP_REGBC = 0x0,
        OP_REGDE = 0x2,
        OP_REGHL = 0x4,
        OP_REGSP = 0x6,
    }

    public static class SymbolHelper
    {
        public static bool IsDecimal(char ch)
        {
            return ch >= '0' && ch <= '9';
        }

        public static bool IsDigit(char ch)
        {
            return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F');
        }

        public static bool IsAlfa(char ch)
        {
            return (ch >= 'A' && ch <= 'Z');
        }

        private static readonly IntBase[] intBases = new IntBase[]
        {
            new IntBase('H', 16),
            new IntBase('O', 8),
            new IntBase('B', 2),
            new IntBase('\0', 10),
        };

        public static int Radix(char suffix)
        {
            foreach (var b in intBases)
            {
                if (b.suffix == suffix)
                    return b.radix;
            }
            return 10;
        }
    }
}
