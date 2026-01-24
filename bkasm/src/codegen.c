#include <stdio.h>
#include "bkasm.h"
#include "codegen.h"
#include "asmast.h"
#include "asmvars.h"
#include "errors.h"
#include "lexer.h"
#include "mathexpr.h"
#include "nodes.h"

unsigned char prog[MAX_PROG_SIZE];
int code_org = 0;

void codegen_generate(Node *node, int pc, int size)
{
    char err_msg[MAX_ERR_MSG_LEN];
    int i;
    Expr *immediate_param = node->u.op.immediate;
    ExprValue immediate_value;

    if(size == 0) return;

    if (node->type == NODE_INSTRUCTION)
    {
        switch(node->u.op.instr_type)
        {
            case TOK_DB:
                prog[pc - code_org] = immediate_param->op.evaluate(immediate_param);
                break;

            case TOK_ORG: // non-executable instructions - implement later TODO
                break;
            default:
                if (size > 1)
                {
                    if (immediate_param == NULL)
                    {
                        sprintf(err_msg, "\nMissing immediate parameter for instruction %s", node->ident);
                        throw_error(E_LINKERERROR, err_msg);
                        exit_nicely(E_LINKERERROR);
                    }
                    else
                        immediate_value = immediate_param->op.evaluate(immediate_param);
                }

                for (i = 0; i < size; i++)
                {
                    switch (i)
                    {
                        case 0:
                            prog[i + pc - code_org] = node->u.op.opcode;
                            break;
                        case 1:
                            prog[i + pc - code_org] = immediate_value;
                            break;
                        case 2:
                            prog[i + pc - code_org] = immediate_value/256;
                            break;
                    }
                }
        }
    }
}

int codegen_evaluate_ast(Node *node, int pc, ASTree *ast)
{
    char err_msg[MAX_ERR_MSG_LEN];
    int size = 0;
    switch (node->type)
    {
    case NODE_INSTRUCTION:
    case NODE_PSEUDO:
        /* Evaluate instructions and params to get size of instruction only, not to generate code.
         * Evaluates params and returns size of instruction
         */

        switch (node->u.op.instr_type)
        {

        // 0 bytes 0 immediate params instructions
        case TOK_CMA:
        case TOK_CMC:
        case TOK_DAA:
        case TOK_DI:
        case TOK_EI:
        case TOK_HLT:
        case TOK_JM:
        case TOK_NOP:
        case TOK_ORI:
        case TOK_PCHL:
        case TOK_RAL:
        case TOK_RAR:
        case TOK_RC:
        case TOK_RET:
        case TOK_RIM:
        case TOK_RLC:
        case TOK_RM:
        case TOK_RNC:
        case TOK_RNZ:
        case TOK_RP:
        case TOK_RPE:
        case TOK_RPO:
        case TOK_RRC:
        case TOK_RZ:
        case TOK_SBI:
        case TOK_SIM:
        case TOK_SPHL:
        case TOK_STC:
        case TOK_SUI:
        case TOK_XCHG:
        case TOK_XRI:
        case TOK_XTHL:
            size = 1;
            break;
        case TOK_MOV:
            node->u.op.opcode |= (node->u.op.lparam->data.value << 3) | node->u.op.rparam->data.value;
            size = 1;
            break;
        case TOK_ADD:
        case TOK_ADC:
        case TOK_ANA:
        case TOK_AND:
        case TOK_CMP:
        case TOK_ORA:
        case TOK_SBB:
        case TOK_SUB:
        case TOK_XRA:
            node->u.op.opcode |= node->u.op.lparam->data.value;
            size = 1;
            break;
        case TOK_INR:
        case TOK_DCR:
            node->u.op.opcode |= (node->u.op.lparam->data.value << 3);
            size = 1;
            break;
        case TOK_DAD:
        case TOK_LDAX:
        case TOK_STAX:
        case TOK_POP:
        case TOK_PUSH:
        case TOK_INX:
        case TOK_DCX:
            node->u.op.opcode |= (node->u.op.lparam->data.value << 4);
            size = 1;
            break;

        // 1 byte 1 immediate param instructions rparam may be NULL
        case TOK_ACI:
        case TOK_ADI:
        case TOK_ANI:
        case TOK_CPI:
        case TOK_MVI:
        case TOK_OUT:
        case TOK_RST:
            node->u.op.lparam->op.evaluate(node->u.op.lparam);
            if (node->u.op.lparam->type != EXPR_REG)
                node->u.op.immediate = node->u.op.lparam;
            if(node->u.op.rparam)
            {
                node->u.op.rparam->op.evaluate(node->u.op.rparam);
                if(node->u.op.rparam->type != EXPR_REG)
                    node->u.op.immediate = node->u.op.rparam;
            }
            size = 2;
            break;

        // 2 bytes one immediate param instructions rparam may be NULL
        case TOK_CALL:
        case TOK_CC:
        case TOK_CM:
        case TOK_CNC:
        case TOK_CNZ:
        case TOK_CP:
        case TOK_CPE:
        case TOK_CPO:
        case TOK_CZ:
        case TOK_IN:
        case TOK_JC:
        case TOK_JMP:
        case TOK_JNC:
        case TOK_JNZ:
        case TOK_JP:
        case TOK_JPE:
        case TOK_JPO:
        case TOK_JZ:
        case TOK_LDA:
        case TOK_LHLD:
        case TOK_LXI:
        case TOK_SHLD:
        case TOK_STA:
            node->u.op.lparam->op.evaluate(node->u.op.lparam);
            if (node->u.op.lparam->type != EXPR_REG)
                node->u.op.immediate = node->u.op.lparam;
            if(node->u.op.rparam)
            {
                node->u.op.rparam->op.evaluate(node->u.op.rparam);
                if(node->u.op.rparam->type != EXPR_REG)
                    node->u.op.immediate = node->u.op.rparam;
            }
            size = 3;
            break;
        case TOK_DB:
            size = 1;
            node->u.op.lparam->op.evaluate(node->u.op.lparam);
            node->u.op.immediate = node->u.op.lparam;
            break;
        case TOK_ORG:
            code_org = node->u.op.lparam->op.evaluate(node->u.op.lparam);
            node->u.op.immediate = node->u.op.lparam;
            size = code_org; 
            break;
        default:
            sprintf(err_msg, "\nUnexpected instruction: unknown opcode %s", node->ident);
            throw_error(E_LINKERERROR, err_msg);
            break;
        }
        break;
    case NODE_VAR:
        switch (node->u.label.target_type)
        {
        case TOK_REGPC:
            asmvars_add(node->ident, pc);
            break;
        case TOK_IDENT:
            asmvars_add(node->ident, node->u.label.target->op.evaluate(node->u.label.target));
            break;
        }
        break;
    default:
        throw_error(E_LINKERERROR, "\nUnknown NodeType");
        break;
    }
    if(bkasm_stage == GENERATE_STAGE && node->type == NODE_INSTRUCTION )
        codegen_generate(node, pc, size);
    return size;
}

unsigned char* codegen_link(ASTree* ast)
{
    int pc = 0;
    int instrSize;
    NodeList *it;
    FILE *file;

    for(bkasm_stage = EVAL_STAGE; bkasm_stage <= GENERATE_STAGE; bkasm_stage ++)
    {
        for(it = ast->firstNode; it; it = it -> next)
        {
            instrSize = codegen_evaluate_ast(&it->node, pc, ast);
            printf("DEBUG: instruction size: %d pc: %d\n", instrSize, pc); // FIXME remove
            pc += instrSize;
        }
        if(bkasm_stage == EVAL_STAGE)
            pc = 0;
    }
    asmvars_print();
    printf("codesize = %d\n", pc - code_org);
    file = fopen("prog.bin", "wb");
    fwrite(prog, 1, pc - code_org, file);
    fclose(file);
    return prog;
}
