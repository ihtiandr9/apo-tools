namespace Bkasm
{
    public class LexerBK : Lexer
    {
        public LexerBK() : base(LexerTable.Bk.WordTable, LexerTable.Bk.SymbolTable)
        {
        }
    }
}
