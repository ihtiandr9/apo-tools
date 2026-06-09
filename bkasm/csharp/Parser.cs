using System;

namespace Bkasm
{
    public abstract class Parser
    {
        public ASTree ast;
        public eErrorType error;

        public Parser()
        {
            ast = new ASTree();
        }

        protected abstract Node ParseOp(Lexer lexer);

        private Node ParseVar(Lexer lexer)
        {
            var m_token = lexer.token;
            string l_ident = m_token.ident;
            lexer.NextTok();
            m_token = lexer.token;
            Node node = null;

            if (m_token.type == eIdentType.TOK_COLON)
            {
                node = new Node(l_ident);
                lexer.SkipOne();
                lexer.SkipWhile(Constants.SPACE);
                lexer.NextTok();
                m_token = lexer.token;
                if (m_token.type == eIdentType.TOK_EQU)
                {
                    lexer.SkipWhile(Constants.SPACE);
                    lexer.NextTok();
                    node.label.target = ParseParam(lexer);
                    node.label.targetType = eIdentType.TOK_IDENT;
                    lexer.SkipUntil(Constants.EOL_SYM);
                    ParseComment(lexer);
                }
                else
                {
                    node.label.target = new RegExpr((int)eIdentType.TOK_REGPC, "PC");
                    node.label.targetType = eIdentType.TOK_REGPC;
                    ast.AddStatement(node);
                    ParseStatement(lexer);
                    node = null;
                }
            }
            else
            {
                ErrorHandler.ThrowError(eErrorType.E_UNKIDENT, l_ident);
                lexer.SkipUntil(Constants.EOL_SYM);
                lexer.SkipOne();
            }
            return node;
        }

        private void ParseComment(Lexer lexer)
        {
            var m_token = lexer.token;
            var currstr = Inbuf.CurrStr();

            switch (m_token.kind)
            {
                case eIdentKind.SYM:
                    switch (m_token.type)
                    {
                        case eIdentType.L_EOL:
                            lexer.SkipOne();
                            break;
                        case eIdentType.L_EOF:
                            lexer.SkipOne();
                            return;
                        case eIdentType.TOK_SEMICOLON:
                            lexer.SkipUntil(Constants.EOL_SYM);
                            lexer.SkipOne();
                            break;
                        default:
                            error = eErrorType.E_UNEXPSYM;
                            Console.Error.WriteLine("In string: {0} {1}", currstr.num, new string(currstr.str));
                            ErrorHandler.ThrowError(eErrorType.E_UNEXPSYM, m_token.ident ?? "");
                            lexer.SkipUntil(Constants.EOL_SYM);
                            break;
                    }
                    break;
                default:
                    error = eErrorType.E_UNEXPSYM;
                    Console.Error.WriteLine("In string: {0} {1}", currstr.num, new string(currstr.str));
                    ErrorHandler.ThrowError(eErrorType.E_UNEXPSYM, m_token.ident ?? "");
                    lexer.SkipUntil(Constants.EOL_SYM);
                    break;
            }
        }

        private Expr ParseTerm(Lexer lexer)
        {
            var m_token = lexer.token;
            Expr result = null;

            switch (m_token.type)
            {
                case eIdentType.TOK_MINUS:
                    lexer.SkipOne();
                    lexer.SkipWhile(Constants.SPACE);
                    lexer.NextTok();
                    result = ParseTerm(lexer);
                    {
                        var zero = new ConstExpr(0);
                        var expr = new AdditionExpr(eIdentType.TOK_MINUS);
                        expr.lparam = zero;
                        expr.rparam = result;
                        result = expr;
                    }
                    break;
                case eIdentType.TOK_NUM:
                    result = new ConstExpr(m_token.value);
                    break;
                case eIdentType.TOK_IDENT:
                    result = new VarExpr(m_token.ident);
                    break;
                case eIdentType.TOK_LPAREN:
                    lexer.SkipOne();
                    lexer.SkipWhile(Constants.SPACE);
                    lexer.NextTok();
                    result = ParseAddition(lexer);
                    if (lexer.token.type == eIdentType.TOK_RPAREN)
                        lexer.SkipOne();
                    else
                        ErrorHandler.ThrowError(eErrorType.E_SYNTAXERROR, ") expected");
                    break;
                default:
                    ErrorHandler.ThrowError(eErrorType.E_UNEXPTOKEN, m_token.ident ?? "");
                    result = new ConstExpr(0);
                    break;
            }
            return result;
        }

        private Expr ParseMultiplication(Lexer lexer)
        {
            Expr result = ParseTerm(lexer);
            lexer.SkipWhile(Constants.SPACE);
            lexer.NextTok();
            var m_token = lexer.token;

            while (m_token.type == eIdentType.TOK_ASTERISK)
            {
                var expr = new MultiplicationExpr(eIdentType.TOK_ASTERISK);
                expr.lparam = result;
                lexer.SkipOne();
                lexer.SkipWhile(Constants.SPACE);
                lexer.NextTok();
                expr.rparam = ParseTerm(lexer);
                result = expr;
                lexer.SkipWhile(Constants.SPACE);
                lexer.NextTok();
                m_token = lexer.token;
            }
            return result;
        }

        private Expr ParseAddition(Lexer lexer)
        {
            Expr result = ParseMultiplication(lexer);
            lexer.SkipWhile(Constants.SPACE);
            lexer.NextTok();
            var m_token = lexer.token;

            while (m_token.type == eIdentType.TOK_PLUS || m_token.type == eIdentType.TOK_MINUS)
            {
                var expr = new AdditionExpr(m_token.type);
                expr.lparam = result;
                lexer.SkipOne();
                lexer.SkipWhile(Constants.SPACE);
                lexer.NextTok();
                expr.rparam = ParseMultiplication(lexer);
                result = expr;
                lexer.SkipWhile(Constants.SPACE);
                lexer.NextTok();
                m_token = lexer.token;
            }
            return result;
        }

        protected Expr ParseDbParam(Lexer lexer)
        {
            Expr expr = null;
            var m_token = lexer.token;

            if (m_token.kind == eIdentKind.STRING)
            {
                string src = m_token.ident;
                int len = m_token.len;
                int i;

                for (i = 0; i < len - 1; i++)
                {
                    expr = new ConstExpr(src[i]);
                    var node = new Node("DB", eIdentType.TOK_DB, 0);
                    node.op.lparam = expr;
                    ast.AddStatement(node);
                }

                if (len > 0)
                    expr = new ConstExpr(src[len - 1]);
                lexer.SkipOne();
            }
            else
            {
                switch (m_token.kind)
                {
                    case eIdentKind.SYM:
                        if (m_token.type == eIdentType.TOK_MINUS)
                            expr = ParseParam(lexer);
                        else
                            expr = null;
                        break;
                    default:
                        expr = ParseParam(lexer);
                        break;
                }
            }
            return expr;
        }

        protected Expr ParseDwParam(Lexer lexer)
        {
            Expr expr = null;
            var m_token = lexer.token;

            switch (m_token.kind)
            {
                case eIdentKind.SYM:
                    if (m_token.type == eIdentType.TOK_MINUS)
                        expr = ParseParam(lexer);
                    else
                        expr = null;
                    break;
                default:
                    expr = ParseParam(lexer);
                    break;
            }
            return expr;
        }

        protected Expr ParseParam(Lexer lexer)
        {
            Expr expr = null;
            var m_token = lexer.token;

            switch (m_token.kind)
            {
                case eIdentKind.REG:
                    expr = new RegExpr(m_token.value, m_token.ident);
                    break;
                case eIdentKind.VAR:
                case eIdentKind.CONST:
                    switch (m_token.type)
                    {
                        case eIdentType.TOK_IDENT:
                        case eIdentType.TOK_NUM:
                            expr = ParseAddition(lexer);
                            break;
                        default:
                            ErrorHandler.ThrowError(eErrorType.E_UNKIDENT, m_token.ident ?? "");
                            break;
                    }
                    break;
                case eIdentKind.SYM:
                    if (m_token.type == eIdentType.TOK_LPAREN || m_token.type == eIdentType.TOK_MINUS)
                        expr = ParseAddition(lexer);
                    else
                    {
                        ErrorHandler.ThrowError(eErrorType.E_UNEXPTOKEN, m_token.ident ?? "");
                        Globals.ExitNicely(eErrorType.E_UNEXPTOKEN);
                    }
                    break;
                default:
                    ErrorHandler.ThrowError(eErrorType.E_UNEXPTOKEN, m_token.ident ?? "");
                    Globals.ExitNicely(eErrorType.E_UNEXPTOKEN);
                    break;
            }
            lexer.SkipWhile(Constants.SPACE);
            return expr;
        }

        private void ParseStatement(Lexer lexer)
        {
            var currstr = Inbuf.CurrStr();
            var m_token = lexer.token;

            switch (m_token.kind)
            {
                case eIdentKind.VAR:
                    switch (m_token.type)
                    {
                        case eIdentType.TOK_IDENT:
                            ast.AddStatement(ParseVar(lexer));
                            break;
                        default:
                            error = eErrorType.E_UNEXPTOKEN;
                            Console.Error.WriteLine("In string: {0} {1}", currstr.num, new string(currstr.str));
                            ErrorHandler.ThrowError(eErrorType.E_UNEXPTOKEN, m_token.ident ?? "");
                            break;
                    }
                    break;

                case eIdentKind.SYM:
                    switch (m_token.type)
                    {
                        case eIdentType.TOK_SEMICOLON:
                            ParseComment(lexer);
                            break;
                        case eIdentType.L_EOL:
                            lexer.SkipOne();
                            break;
                        case eIdentType.L_EOF:
                            lexer.SkipOne();
                            break;
                        default:
                            error = eErrorType.E_UNEXPSYM;
                            Console.Error.WriteLine("In string: {0} {1}", currstr.num, new string(currstr.str));
                            ErrorHandler.ThrowError(eErrorType.E_UNEXPSYM, m_token.ident ?? "");
                            Globals.ExitNicely(eErrorType.E_UNEXPSYM);
                            break;
                    }
                    break;

                case eIdentKind.OP:
                    ast.AddStatement(ParseOp(lexer));
                    lexer.NextTok();
                    ParseComment(lexer);
                    break;

                case eIdentKind.INT:
                {
                    m_token = lexer.token;
                    if (m_token.type == eIdentType.TOK_INCLUDE)
                    {
                        lexer.SkipWhile(Constants.SPACE);
                        lexer.NextTok();
                        m_token = lexer.token;
                        if (m_token.kind != eIdentKind.STRING)
                        {
                            error = eErrorType.E_SYNTAXERROR;
                            Console.Error.WriteLine("In string: {0} {1}", currstr.num, new string(currstr.str));
                            ErrorHandler.ThrowError(eErrorType.E_SYNTAXERROR, "expected filename after INCLUDE");
                            Globals.ExitNicely(eErrorType.E_SYNTAXERROR);
                        }
                        if (Inbuf.PushFile(m_token.ident) != 0)
                        {
                            error = eErrorType.E_SYNTAXERROR;
                            Console.Error.WriteLine("In string: {0} {1}", currstr.num, new string(currstr.str));
                            Console.Error.WriteLine("Error: Cannot open include file: {0}", m_token.ident);
                            Globals.ExitNicely(eErrorType.E_SYNTAXERROR);
                        }
                        lexer.CurrentChar = (byte)Constants.CH_NULL;
                        break;
                    }
                    {
                        var internal_node = new Node("ENDPRG");
                        if (m_token.type == eIdentType.TOK_END)
                        {
                            internal_node.label.target = new RegExpr((int)eIdentType.TOK_REGPC, "PC");
                            internal_node.label.targetType = eIdentType.TOK_REGPC;
                            ast.AddStatement(internal_node);
                            lexer.SkipOne();
                            lexer.SkipWhile(Constants.SPACE);
                            lexer.NextTok();
                            ParseStatement(lexer);
                            internal_node = null;
                        }
                    }
                    break;
                }

                case eIdentKind.KIND_NONE:
                    lexer.SkipUntil(Constants.EOL_SYM);
                    lexer.SkipOne();
                    break;

                default:
                    error = eErrorType.E_UNEXPTOKEN;
                    Console.Error.WriteLine("In string: {0} {1}", currstr.num, new string(currstr.str));
                    ErrorHandler.ThrowError(eErrorType.E_UNEXPTOKEN, m_token.ident ?? "");
                    break;
            }
        }

        public void Parse(Lexer lexer)
        {
            while (lexer.NextTok() != 0)
            {
                ParseStatement(lexer);
                lexer.SkipWhile(Constants.SPACE);
            }
        }
    }
}
