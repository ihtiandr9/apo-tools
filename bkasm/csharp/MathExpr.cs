namespace Bkasm
{
    public abstract class Expr
    {
        public eExprType type;
        public string ident;
	public abstract int value { get; }
    }

    public class ConstExpr : Expr
    {
        private int data;

        public ConstExpr(int v)
        {
            type = eExprType.EXPR_CONST;
            data = v;
            ident = null;
        }

	public override int value => data;
    }

    public class RegExpr : Expr
    {
        private int data;

        public RegExpr(int v, string id)
        {
            type = eExprType.EXPR_REG;
            data = v;
            int len = id.Length;
            if (len > Constants.MAX_LABEL_SIZE)
                len = Constants.MAX_LABEL_SIZE;
            ident = id.Substring(0, len);
        }

        public override int value => data;
    }

    public class VarExpr : Expr
    {
        public VarExpr(string id)
        {
            type = eExprType.EXPR_VAR;
            int len = id.Length;
            if (len > Constants.MAX_LABEL_SIZE)
                len = Constants.MAX_LABEL_SIZE;
            ident = id.Substring(0, len);
        }

        public override int value
        {
            get
            {
                if (ident != null)
                {
                    int val;
                    if (AsmVars.Value(ident, out val))
                        return val;
                    if (Globals.bkasm_stage > BkasmStage.PARSE_STAGE)
                    {
                        ErrorHandler.ThrowError(eErrorType.E_LINKERERROR, "\nUndefined variable " + ident + "\n");
                    }
                }
                return 0;
            }
        }
    }

    public abstract class MathExpr : Expr
    {
        public Expr lparam;
        public Expr rparam;
        public eIdentType opcode;

        public MathExpr(eIdentType op)
        {
            type = eExprType.EXPR_MATH;
            opcode = op;
            ident = null;
            lparam = null;
            rparam = null;
        }
    }

    public class AdditionExpr : MathExpr
    {
        public AdditionExpr(eIdentType operation) : base(operation) { }

	public override int value
	{
	    get
	    {
		if (lparam == null || rparam == null)
		{
		    ErrorHandler.ThrowError(eErrorType.E_SYNTAXERROR, " invalid addition operation");
		    return 0;
		}

		int lv = lparam.value;
		int rv = rparam.value;
		int result = 0;

		switch (opcode)
		{
		    case eIdentType.TOK_PLUS:
			result = lv + rv;
			break;
		    case eIdentType.TOK_MINUS:
			result = lv - rv;
			break;
		    default:
			ErrorHandler.ThrowError(eErrorType.E_SYNTAXERROR, " invalid operation");
			return 0;
		}

		return result;
	    }
	}
    }

    public class MultiplicationExpr : MathExpr
    {
        public MultiplicationExpr(eIdentType operation) : base(operation) { }

        public override int value
        {
	    get
	    {
		if (lparam == null || rparam == null)
		{
		    ErrorHandler.ThrowError(eErrorType.E_SYNTAXERROR, " invalid multiplication operation");
		    return 0;
		}

		int lv = lparam.value;
		int rv = rparam.value;
		int result = 0;

		switch (opcode)
		{
		    case eIdentType.TOK_ASTERISK:
			result = lv * rv;
			break;
		    default:
			ErrorHandler.ThrowError(eErrorType.E_SYNTAXERROR, " invalid operation");
			return 0;
		}

		return result;
	    }
	}
    }
}
