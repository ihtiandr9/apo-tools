using System;

namespace Bkasm
{
    public static class CodeGen
    {
        private static byte[] prog = new byte[Constants.MAX_PROG_SIZE];
        private static int codeOrg = 0;

        private static void Generate(Node node, int pc, int size)
        {
            if (size == 0) return;

            if (pc - codeOrg + size > Constants.MAX_PROG_SIZE)
            {
                ErrorHandler.ThrowError(eErrorType.E_LINKERERROR, "\nProgram size exceeds " + Constants.MAX_PROG_SIZE + " bytes\n");
                Globals.ExitNicely(eErrorType.E_LINKERERROR);
            }

            if (node.type == eNodeType.NODE_INSTRUCTION)
            {
                switch (node.op.instrType)
                {
                    case eIdentType.TOK_DB:
                    {
                        var imm = node.op.immediate;
                        if (imm == null)
                        {
                            ErrorHandler.ThrowError(eErrorType.E_LINKERERROR, "\nMissing immediate parameter for DB");
                            Globals.ExitNicely(eErrorType.E_LINKERERROR);
                        }
                        int db_val = imm.value;
                        if (db_val < -128 || db_val > 255)
                        {
                            ErrorHandler.ThrowError(eErrorType.E_LINKERERROR, "\nDB value " + db_val + " out of range -128-255\n");
                            Globals.ExitNicely(eErrorType.E_LINKERERROR);
                        }
                        prog[pc - codeOrg] = (byte)db_val;
                        break;
                    }

                    case eIdentType.TOK_DW:
                    {
                        var imm = node.op.immediate;
                        if (imm == null)
                        {
                            ErrorHandler.ThrowError(eErrorType.E_LINKERERROR, "\nMissing immediate parameter for DW");
                            Globals.ExitNicely(eErrorType.E_LINKERERROR);
                        }
                        int imm_val = imm.value;
                        prog[pc - codeOrg] = (byte)imm_val;
                        prog[pc - codeOrg + 1] = (byte)(imm_val / 256);
                        break;
                    }

                    case eIdentType.TOK_ORG:
                        break;

                    default:
                    {
                        int immediate_value = 0;
                        if (size > 1)
                        {
                            var imm = node.op.immediate;
                            if (imm == null)
                            {
                                ErrorHandler.ThrowError(eErrorType.E_LINKERERROR, "\nMissing immediate parameter for instruction " + node.ident);
                                Globals.ExitNicely(eErrorType.E_LINKERERROR);
                            }
                            else
                                immediate_value = imm.value;

                            if (size == 2 && (immediate_value < -128 || immediate_value > 255))
                            {
                                ErrorHandler.ThrowError(eErrorType.E_LINKERERROR, "\nImmediate value " + immediate_value + " out of range -128-255 for " + node.ident + "\n");
                                Globals.ExitNicely(eErrorType.E_LINKERERROR);
                            }
                        }

                        for (int i = 0; i < size; i++)
                        {
                            switch (i)
                            {
                                case 0:
                                    prog[i + pc - codeOrg] = (byte)node.op.opcode;
                                    break;
                                case 1:
                                    prog[i + pc - codeOrg] = (byte)immediate_value;
                                    break;
                                case 2:
                                    prog[i + pc - codeOrg] = (byte)(immediate_value / 256);
                                    break;
                            }
                        }
                        break;
                    }
                }
            }
        }

        private static int EvaluateAst(Node node, int pc, ASTree ast)
        {
            int size = 0;

            switch (node.type)
            {
                case eNodeType.NODE_INSTRUCTION:
                case eNodeType.NODE_PSEUDO:
                    switch (node.op.instrType)
                    {
                        case eIdentType.TOK_CMA: case eIdentType.TOK_CMC: case eIdentType.TOK_DAA:
                        case eIdentType.TOK_DI: case eIdentType.TOK_EI: case eIdentType.TOK_HLT:
                        case eIdentType.TOK_NOP: case eIdentType.TOK_PCHL: case eIdentType.TOK_RAL:
                        case eIdentType.TOK_RAR: case eIdentType.TOK_RC: case eIdentType.TOK_RET:
                        case eIdentType.TOK_RIM: case eIdentType.TOK_RLC: case eIdentType.TOK_RM:
                        case eIdentType.TOK_RNC: case eIdentType.TOK_RNZ: case eIdentType.TOK_RP:
                        case eIdentType.TOK_RPE: case eIdentType.TOK_RPO: case eIdentType.TOK_RRC:
                        case eIdentType.TOK_RZ: case eIdentType.TOK_SIM: case eIdentType.TOK_SPHL:
                        case eIdentType.TOK_STC: case eIdentType.TOK_XCHG: case eIdentType.TOK_XTHL:
                            size = 1;
                            break;

                        case eIdentType.TOK_MOV:
                            var lreg = node.op.lparam as RegExpr;
                            var rreg = node.op.rparam as RegExpr;
                            if (lreg != null && rreg != null)
                                node.op.opcode |= (lreg.value << 3) | rreg.value;
                            size = 1;
                            break;

                        case eIdentType.TOK_ADD: case eIdentType.TOK_ADC: case eIdentType.TOK_ANA:
                        case eIdentType.TOK_AND: case eIdentType.TOK_CMP: case eIdentType.TOK_ORA:
                        case eIdentType.TOK_SBB: case eIdentType.TOK_SUB: case eIdentType.TOK_XRA:
                            if (node.op.lparam is RegExpr areg)
                                node.op.opcode |= areg.value;
                            size = 1;
                            break;

                        case eIdentType.TOK_INR: case eIdentType.TOK_DCR:
                            if (node.op.lparam is RegExpr inr)
                                node.op.opcode |= (inr.value << 3);
                            size = 1;
                            break;

                        case eIdentType.TOK_DAD: case eIdentType.TOK_LDAX: case eIdentType.TOK_STAX:
                        case eIdentType.TOK_POP: case eIdentType.TOK_PUSH: case eIdentType.TOK_INX:
                        case eIdentType.TOK_DCX:
                            if (node.op.lparam is RegExpr pair)
                                node.op.opcode |= (pair.value << 3);
                            size = 1;
                            break;

                        case eIdentType.TOK_MVI:
                            if (node.op.lparam is RegExpr mvi)
                                node.op.opcode |= (mvi.value << 3);
                            goto case eIdentType.TOK_ACI;

                        case eIdentType.TOK_ACI: case eIdentType.TOK_ADI: case eIdentType.TOK_ANI:
                        case eIdentType.TOK_CPI: case eIdentType.TOK_ORI: case eIdentType.TOK_SBI:
                        case eIdentType.TOK_SUI: case eIdentType.TOK_XRI: case eIdentType.TOK_OUT:
                        case eIdentType.TOK_IN:
                            if (node.op.lparam != null)
                            {
                                _ = node.op.lparam.value;
                                if (node.op.lparam.type != eExprType.EXPR_REG)
                                    node.op.immediate = node.op.lparam;
                            }
                            if (node.op.rparam != null)
                            {
                                _ = node.op.rparam.value;
                                if (node.op.rparam.type != eExprType.EXPR_REG)
                                    node.op.immediate = node.op.rparam;
                            }
                            size = 2;
                            break;

                        case eIdentType.TOK_RST:
                        {
                            int rst_val = node.op.lparam.value;
                            if (rst_val < 0 || rst_val > 7)
                            {
                                ErrorHandler.ThrowError(eErrorType.E_LINKERERROR, "\nRST value " + rst_val + " out of range 0-7\n");
                                Globals.ExitNicely(eErrorType.E_LINKERERROR);
                            }
                            node.op.opcode |= (rst_val << 3);
                            size = 1;
                            break;
                        }

                        case eIdentType.TOK_CALL: case eIdentType.TOK_CC: case eIdentType.TOK_CM:
                        case eIdentType.TOK_CNC: case eIdentType.TOK_CNZ: case eIdentType.TOK_CP:
                        case eIdentType.TOK_CPE: case eIdentType.TOK_CPO: case eIdentType.TOK_CZ:
                        case eIdentType.TOK_JC: case eIdentType.TOK_JM: case eIdentType.TOK_JMP:
                        case eIdentType.TOK_JNC: case eIdentType.TOK_JNZ: case eIdentType.TOK_JP:
                        case eIdentType.TOK_JPE: case eIdentType.TOK_JPO: case eIdentType.TOK_JZ:
                        case eIdentType.TOK_LDA: case eIdentType.TOK_LHLD: case eIdentType.TOK_SHLD:
                        case eIdentType.TOK_STA:
                            if (node.op.lparam != null)
                            {
                                _ = node.op.lparam.value;
                                if (node.op.lparam.type != eExprType.EXPR_REG)
                                    node.op.immediate = node.op.lparam;
                            }
                            if (node.op.rparam != null)
                            {
                                _ = node.op.rparam.value;
                                if (node.op.rparam.type != eExprType.EXPR_REG)
                                    node.op.immediate = node.op.rparam;
                            }
                            size = 3;
                            break;

                        case eIdentType.TOK_LXI:
                            if (node.op.lparam is RegExpr lxi)
                                node.op.opcode |= (lxi.value << 3);
                            if (node.op.lparam != null)
                            {
                                _ = node.op.lparam.value;
                                if (node.op.lparam.type != eExprType.EXPR_REG)
                                    node.op.immediate = node.op.lparam;
                            }
                            if (node.op.rparam != null)
                            {
                                _ = node.op.rparam.value;
                                if (node.op.rparam.type != eExprType.EXPR_REG)
                                    node.op.immediate = node.op.rparam;
                            }
                            size = 3;
                            break;

                        case eIdentType.TOK_DB:
                            size = 1;
                            if (node.op.lparam != null)
                            {
                                _ = node.op.lparam.value;
                                node.op.immediate = node.op.lparam;
                            }
                            break;

                        case eIdentType.TOK_DW:
                            size = 2;
                            if (node.op.lparam != null)
                            {
                                _ = node.op.lparam.value;
                                node.op.immediate = node.op.lparam;
                            }
                            break;

                        case eIdentType.TOK_ORG:
                            codeOrg = node.op.lparam.value;
                            node.op.immediate = node.op.lparam;
                            size = 0;
                            break;

                        case eIdentType.TOK_DS:
                            size = node.op.lparam.value;
                            node.op.immediate = node.op.lparam;
                            break;

                        default:
                            ErrorHandler.ThrowError(eErrorType.E_LINKERERROR, "\nUnexpected instruction: unknown opcode " + node.ident);
                            break;
                    }
                    break;

                case eNodeType.NODE_VAR:
                    switch (node.label.targetType)
                    {
                        case eIdentType.TOK_REGPC:
                            AsmVars.Add(node.ident, pc);
                            break;
                        case eIdentType.TOK_IDENT:
                            AsmVars.Add(node.ident, node.label.target.value);
                            break;
                    }
                    break;

                default:
                    ErrorHandler.ThrowError(eErrorType.E_LINKERERROR, "\nUnknown NodeType");
                    break;
            }

            if (Globals.bkasm_stage == BkasmStage.GENERATE_STAGE && node.type == eNodeType.NODE_INSTRUCTION)
                Generate(node, pc, size);

            return size;
        }

        public static byte[] Link(ASTree ast)
        {
            int pc = 0;
            int instrSize;

            for (Globals.bkasm_stage = BkasmStage.EVAL_STAGE;
                 Globals.bkasm_stage <= BkasmStage.GENERATE_STAGE;
                 Globals.bkasm_stage++)
            {
                for (var it = ast.firstNode; it != null; it = it.next)
                {
                    instrSize = EvaluateAst(it.node, pc, ast);
                    if (it.node.type == eNodeType.NODE_PSEUDO && it.node.op.instrType == eIdentType.TOK_ORG)
                        pc = codeOrg;
                    else
                        pc += instrSize;
                }
                if (Globals.bkasm_stage == BkasmStage.EVAL_STAGE)
                    pc = 0;
            }

            AsmVars.Print();
            Console.WriteLine("codesize = {0}", pc - codeOrg);

            int dataSize = pc - codeOrg;
            byte[] result = new byte[dataSize];
            Array.Copy(prog, 0, result, 0, dataSize);
            return result;
        }
    }
}
