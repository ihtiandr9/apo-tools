using System;

namespace Bkasm
{
    public struct Lexema
    {
        public eIdentKind kind;
        public eIdentType type;
        public string ident;
        public int value;
        public int len;

        public Lexema(eIdentKind k, eIdentType t, string id, int v, int l)
        {
            kind = k; type = t; ident = id; value = v; len = l;
        }
    }

    public class Lexer
    {
        private byte ch;
        public Lexema token;
        private Lexema[] wordTable;
        private Lexema[] symbolTable;

        public byte CurrentChar { get { return ch; } set { ch = value; } }

        public Lexer(Lexema[] wordTable, Lexema[] symbolTable)
        {
            this.wordTable = wordTable;
            this.symbolTable = symbolTable;
        }

        public int NextTok()
        {
            int m_ch = ch;
            int f_result = 1;

            token.type = eIdentType.TOK_NONE;
            token.value = 0;
            token.len = 0;
            token.ident = null;

            var currstr = Inbuf.CurrStr();

            while (token.type == eIdentType.TOK_NONE)
            {
                if (m_ch == Constants.CH_NULL)
                    m_ch = Inbuf.NextChar();
                if (m_ch == Constants.EOF_SYM)
                {
                    token.type = eIdentType.L_EOF;
                    token.kind = eIdentKind.SYM;
                    continue;
                }

                Lexema? m_sym = null;
                foreach (var s in symbolTable)
                {
                    if (s.ident != null && s.ident.Length > 0 && (byte)s.ident[0] == m_ch)
                    {
                        m_sym = s;
                        break;
                    }
                }

                if (m_sym != null && m_sym.Value.type != eIdentType.TOK_NONE)
                {
                    token.kind = m_sym.Value.kind;
                    token.type = m_sym.Value.type;
                    token.ident = m_sym.Value.ident;
                    token.value = m_sym.Value.value;
                    token.len = m_sym.Value.len;
                    continue;
                }

                if (SymbolHelper.IsDecimal((char)m_ch))
                {
                    string ident = "";
                    while (SymbolHelper.IsDigit((char)m_ch))
                    {
                        ident += (char)m_ch;
                        m_ch = Inbuf.NextChar();
                    }
                    token.len = ident.Length;
                    token.ident = null;
                    token.type = eIdentType.TOK_NUM;
                    token.kind = eIdentKind.CONST;
                    int radix = SymbolHelper.Radix((char)m_ch);
                    try
                    {
                        token.value = Convert.ToInt32(ident, radix);
                    }
                    catch
                    {
                        token.value = 0;
                    }
                    continue;
                }

                if (SymbolHelper.IsAlfa((char)m_ch))
                {
                    string ident = "";
                    while (SymbolHelper.IsAlfa((char)m_ch) || SymbolHelper.IsDigit((char)m_ch))
                    {
                        ident += (char)m_ch;
                        m_ch = Inbuf.NextChar();
                    }

                    Lexema? keyword = null;
                    foreach (var w in wordTable)
                    {
                        if (w.ident != null && ident == w.ident)
                        {
                            keyword = w;
                            break;
                        }
                    }

                    if (keyword != null)
                    {
                        token.kind = keyword.Value.kind;
                        token.type = keyword.Value.type;
                        token.ident = keyword.Value.ident;
                        token.value = keyword.Value.value;
                        token.len = keyword.Value.len;
                        continue;
                    }

                    if (ident.Length < 255)
                    {
                        token.kind = eIdentKind.VAR;
                        token.type = eIdentType.TOK_IDENT;
                        token.ident = ident;
                        token.len = ident.Length;
                        continue;
                    }

                    ErrorHandler.ThrowError(eErrorType.E_UNKIDENT, ident);
                    Globals.ExitNicely(eErrorType.E_UNKIDENT);
                }

                if (m_ch == '"')
                {
                    m_ch = Inbuf.NextChar();
                    string ident = "";
                    while (m_ch != '"' && m_ch != Constants.EOF_SYM)
                    {
                        ident += (char)m_ch;
                        m_ch = Inbuf.NextChar();
                    }
                    if (m_ch == Constants.EOF_SYM)
                    {
                        ErrorHandler.ThrowError(eErrorType.E_UNEXPSYM, "\"");
                        Globals.ExitNicely(eErrorType.E_UNEXPSYM);
                    }
                    token.kind = eIdentKind.STRING;
                    token.type = eIdentType.TOK_NONE;
                    token.ident = ident;
                    token.len = ident.Length;
                    break;
                }

                Console.Error.WriteLine("In string: {0} {1}", currstr.num, new string(currstr.str));
                ErrorHandler.ThrowError(eErrorType.E_UNEXPSYM, (char)m_ch);
                token.kind = eIdentKind.KIND_NONE;
                token.type = eIdentType.TOK_NONE;
                token.ident = null;
                token.len = 0;
                break;
            }

            ch = (byte)m_ch;
            if (token.type == eIdentType.L_EOF)
                f_result = 0;

            return f_result;
        }

        public void SkipOne()
        {
            ch = Inbuf.NextChar();
        }

        public void SkipWhile(byte symbol)
        {
            while (ch == symbol && ch != Constants.EOF_SYM)
                ch = Inbuf.NextChar();
        }

        public void SkipUntil(byte symbol)
        {
            while (ch != symbol && ch != Constants.EOF_SYM)
                SkipOne();
            NextTok();
        }
    }
}
