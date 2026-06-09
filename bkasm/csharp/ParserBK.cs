using System;

namespace Bkasm
{
    public class ParserBK : Parser
    {
        private bool IsRegisterPair(Expr expr, int arith)
        {
            if (!(expr is RegExpr reg))
                return false;
            var rv = reg.value;
            return rv == (int)opRegs.OP_REGB ||
                   rv == (int)opRegs.OP_REGC ||
                   rv == (int)opRegs.OP_REGD ||
                   rv == (int)opRegs.OP_REGE ||
                   rv == (int)opRegs.OP_REGH ||
                   rv == (int)opRegs.OP_REGL ||
                   rv == (int)opRegs.OP_REGM ||
                   rv == (int)opRegs.OP_REGA ||
                   rv == (int)opRegPairs.OP_REGBC ||
                   rv == (int)opRegPairs.OP_REGDE ||
                   rv == (int)opRegPairs.OP_REGHL ||
                   (rv == (int)opRegPairs.OP_REGSP && arith != 0 && expr.ident == "SP") ||
                   (rv == (int)opRegPairs.OP_REGSP && arith == 0 && expr.ident == "PSW");
        }

        protected override Node ParseOp(Lexer lexer)
        {
            var op_token_ident = lexer.token.ident;
            var op_token_type = lexer.token.type;
            var op_token_value = lexer.token.value;
            var node = new Node(op_token_ident, op_token_type, op_token_value);
            var currstr = Inbuf.CurrStr();

            switch (op_token_type)
            {
                case eIdentType.TOK_DB:
                {
                    lexer.SkipWhile(Constants.SPACE);
                    lexer.NextTok();
                    var parsed_param = ParseDbParam(lexer);
                    while (parsed_param != null)
                    {
                        if (node == null)
                            node = new Node(op_token_ident, op_token_type, op_token_value);

                        node.op.lparam = parsed_param;
                        ast.AddStatement(node);
                        node = null;

                        lexer.SkipWhile(Constants.COMMA);
                        lexer.SkipWhile(Constants.SPACE);
                        lexer.NextTok();
                        parsed_param = ParseDbParam(lexer);
                    }
                    if (node != null)
                        node = null;
                    break;
                }

                case eIdentType.TOK_DW:
                {
                    lexer.SkipWhile(Constants.SPACE);
                    lexer.NextTok();
                    var parsed_param = ParseDwParam(lexer);
                    while (parsed_param != null)
                    {
                        if (node == null)
                            node = new Node(op_token_ident, op_token_type, op_token_value);

                        node.op.lparam = parsed_param;
                        ast.AddStatement(node);
                        node = null;

                        lexer.SkipWhile(Constants.COMMA);
                        lexer.SkipWhile(Constants.SPACE);
                        lexer.NextTok();
                        parsed_param = ParseDwParam(lexer);
                    }
                    if (node != null)
                        node = null;
                    break;
                }

                case eIdentType.TOK_LXI:
                case eIdentType.TOK_MOV:
                case eIdentType.TOK_MVI:
                {
                    var op = node.op;
                    lexer.SkipWhile(Constants.SPACE);
                    lexer.NextTok();
                    op.lparam = ParseParam(lexer);
                    lexer.SkipWhile(Constants.COMMA);
                    lexer.SkipWhile(Constants.SPACE);
                    lexer.NextTok();
                    op.rparam = ParseParam(lexer);
                    lexer.SkipWhile(Constants.SPACE);

                    if (op_token_type == eIdentType.TOK_MOV)
                    {
                        if (op.lparam.type != eExprType.EXPR_REG || op.rparam.type != eExprType.EXPR_REG)
                        {
                            ErrorHandler.ThrowError(eErrorType.E_SYNTAXERROR, "\nBoth operands of MOV must be registers");
                        }
                    }
                    else if (op_token_type == eIdentType.TOK_MVI)
                    {
                        if (op.lparam.type != eExprType.EXPR_REG)
                        {
                            ErrorHandler.ThrowError(eErrorType.E_SYNTAXERROR, "\nLeft operand of MVI must be a register");
                        }
                        if (op.rparam.type == eExprType.EXPR_REG)
                        {
                            ErrorHandler.ThrowError(eErrorType.E_SYNTAXERROR, "\nRight operand of MVI must be an immediate value");
                        }
                    }
                    break;
                }

                case eIdentType.TOK_ACI: case eIdentType.TOK_ADC: case eIdentType.TOK_ADD:
                case eIdentType.TOK_ADI: case eIdentType.TOK_ANA: case eIdentType.TOK_AND:
                case eIdentType.TOK_ANI: case eIdentType.TOK_CALL: case eIdentType.TOK_CC:
                case eIdentType.TOK_CM: case eIdentType.TOK_CMP: case eIdentType.TOK_CNC:
                case eIdentType.TOK_CNZ: case eIdentType.TOK_CP: case eIdentType.TOK_CPE:
                case eIdentType.TOK_CPI: case eIdentType.TOK_CPO: case eIdentType.TOK_CZ:
                case eIdentType.TOK_DAD: case eIdentType.TOK_DCR: case eIdentType.TOK_DCX:
                case eIdentType.TOK_IN: case eIdentType.TOK_INR: case eIdentType.TOK_INX:
                case eIdentType.TOK_JC: case eIdentType.TOK_JM: case eIdentType.TOK_JMP:
                case eIdentType.TOK_JNC: case eIdentType.TOK_JNZ: case eIdentType.TOK_JP:
                case eIdentType.TOK_JPE: case eIdentType.TOK_JPO: case eIdentType.TOK_JZ:
                case eIdentType.TOK_LDA: case eIdentType.TOK_LDAX: case eIdentType.TOK_LHLD:
                case eIdentType.TOK_ORA: case eIdentType.TOK_ORI: case eIdentType.TOK_OUT:
                case eIdentType.TOK_POP: case eIdentType.TOK_PUSH: case eIdentType.TOK_RST:
                case eIdentType.TOK_SBB: case eIdentType.TOK_SBI: case eIdentType.TOK_SHLD:
                case eIdentType.TOK_STA: case eIdentType.TOK_STAX: case eIdentType.TOK_SUB:
                case eIdentType.TOK_SUI: case eIdentType.TOK_XRA: case eIdentType.TOK_XRI:
                {
                    var op = node.op;
                    lexer.SkipWhile(Constants.SPACE);
                    lexer.NextTok();
                    op.lparam = ParseParam(lexer);
                    break;
                }

                case eIdentType.TOK_ORG:
                case eIdentType.TOK_DS:
                {
                    node.type = eNodeType.NODE_PSEUDO;
                    var op = node.op;
                    lexer.SkipWhile(Constants.SPACE);
                    lexer.NextTok();
                    op.lparam = ParseParam(lexer);
                    break;
                }

                case eIdentType.TOK_CMA: case eIdentType.TOK_CMC: case eIdentType.TOK_DAA:
                case eIdentType.TOK_DI: case eIdentType.TOK_EI: case eIdentType.TOK_END:
                case eIdentType.TOK_HLT: case eIdentType.TOK_NOP: case eIdentType.TOK_PCHL:
                case eIdentType.TOK_RAL: case eIdentType.TOK_RAR: case eIdentType.TOK_RC:
                case eIdentType.TOK_RET: case eIdentType.TOK_RIM: case eIdentType.TOK_RLC:
                case eIdentType.TOK_RM: case eIdentType.TOK_RNC: case eIdentType.TOK_RNZ:
                case eIdentType.TOK_RP: case eIdentType.TOK_RPE: case eIdentType.TOK_RPO:
                case eIdentType.TOK_RRC: case eIdentType.TOK_RZ: case eIdentType.TOK_SIM:
                case eIdentType.TOK_SPHL: case eIdentType.TOK_STC: case eIdentType.TOK_XCHG:
                case eIdentType.TOK_XTHL:
                {
                    lexer.SkipWhile(Constants.SPACE);
                    break;
                }

                case eIdentType.TOK_SEMICOLON:
                    break;

                default:
                    error = eErrorType.E_UNKKEYWORD;
                    Console.Error.WriteLine("In string: {0} {1}", currstr.num, new string(currstr.str));
                    ErrorHandler.ThrowError(eErrorType.E_UNKKEYWORD, op_token_ident ?? "");
                    lexer.SkipUntil(Constants.EOL_SYM);
                    break;
            }

            if (node != null && node.type == eNodeType.NODE_INSTRUCTION)
            {
                var op = node.op;
                switch (op_token_type)
                {
                    case eIdentType.TOK_ADD: case eIdentType.TOK_ADC: case eIdentType.TOK_ANA:
                    case eIdentType.TOK_CMP: case eIdentType.TOK_ORA: case eIdentType.TOK_SBB:
                    case eIdentType.TOK_SUB: case eIdentType.TOK_XRA:
                        if (op.lparam.type != eExprType.EXPR_REG)
                            ErrorHandler.ThrowError(eErrorType.E_SYNTAXERROR, "\nOperand of " + op_token_ident + " must be a register");
                        break;
                    case eIdentType.TOK_INR: case eIdentType.TOK_DCR:
                        if (op.lparam.type != eExprType.EXPR_REG)
                            ErrorHandler.ThrowError(eErrorType.E_SYNTAXERROR, "\nOperand of " + op_token_ident + " must be a register");
                        break;
                    case eIdentType.TOK_DAD: case eIdentType.TOK_LDAX: case eIdentType.TOK_STAX:
                    case eIdentType.TOK_INX: case eIdentType.TOK_DCX:
                        if (!IsRegisterPair(op.lparam, 1))
                            ErrorHandler.ThrowError(eErrorType.E_SYNTAXERROR, "\nOperand of " + op_token_ident + " must be a register pair");
                        break;
                    case eIdentType.TOK_POP: case eIdentType.TOK_PUSH:
                        if (!IsRegisterPair(op.lparam, 0))
                            ErrorHandler.ThrowError(eErrorType.E_SYNTAXERROR, "\nOperand of " + op_token_ident + " must be a register pair");
                        break;
                }
            }

            return node;
        }
    }
}
