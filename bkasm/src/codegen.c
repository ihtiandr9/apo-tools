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
    Expr *immediate_param = node->op.immediate;

    

    if (node->type == NODE_INSTRUCTION)
    {
        if (node->op.opcode == TOK_DB || node->op.opcode == TOK_ORG) // non-executable instructions - implement later TODO
            ;
        else
        {
            if (size > 1 && immediate_param == NULL)
            {
                sprintf(err_msg, "Missing immediate parameter for instruction %s\n", node->op.ident);
                throw_error(E_LINKERERROR, err_msg);
                exit_nicely(E_LINKERERROR);
            }

            for (i = 0; i < size; i++)
            {
                switch (i)
                {
                case 0:
                    prog[i + pc - code_org] = node->op.opcode;
                    break;
                case 1:
                    prog[i + pc - code_org] = immediate_param->op.evaluate(immediate_param);
                    break;
                case 2:
                    prog[i + pc - code_org] = immediate_param->op.evaluate(immediate_param) / 256;
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
    case NODE_INSTRUCTION:  // Evaluate instructions and params to get size of instruction only, not to generate code.
                            // Evaluates params and returns size of instruction
        switch (node->op.opcode)
        {
        case TOK_ADD: // 0 bytes 0 immediate params instructions
        case TOK_AND:
        case TOK_DCR:
        case TOK_DCX:
        case TOK_EI:
        case TOK_INR:
        case TOK_INX:
        case TOK_LHLD:
        case TOK_MOV:
        case TOK_NOP:
        case TOK_RET:
        case TOK_RAL:
        case TOK_RLC:
        case TOK_SUB:
        case TOK_STAX:
        case TOK_DAD:
        case TOK_LDAX:
        case TOK_XCHG:
            size = 1;
            break;
        case TOK_ANI: // 1 byte 1 immediate param instructions rparam may be NULL
        case TOK_MVI:
            node->op.lparam->op.evaluate(node->op.lparam);
            if (node->op.lparam->type != EXPR_REG)
                node->op.immediate = node->op.lparam;
            if(node->op.rparam)
            {
                node->op.rparam->op.evaluate(node->op.rparam);
                if(node->op.rparam->type != EXPR_REG)
                    node->op.immediate = node->op.rparam;
            }
            size = 2;
            break;
        case TOK_CALL: // 2 bytes one immediate param instructions rparam may be NULL
        case TOK_JMP:
        case TOK_JZ:
        case TOK_LXI:
            node->op.lparam->op.evaluate(node->op.lparam);
            if (node->op.lparam->type != EXPR_REG)
                node->op.immediate = node->op.lparam;
            if(node->op.rparam)
            {
                node->op.rparam->op.evaluate(node->op.rparam);
                if(node->op.rparam->type != EXPR_REG)
                    node->op.immediate = node->op.rparam;
            }
            size = 3;
            break;
        case TOK_DB:
            size = 1;
            node->op.lparam->op.evaluate(node->op.lparam);
            node->op.immediate = node->op.lparam;
            break;
        case TOK_ORG:
            code_org = node->op.lparam->op.evaluate(node->op.lparam);
            node->op.immediate = node->op.lparam;
            size = code_org; 
            break;
        default:
            sprintf(err_msg, "Unexpected instruction:\n unknown opcode %s\n", node->op.ident);
            throw_error(E_LINKERERROR, err_msg);
            break;
        }
        break;
    case NODE_VAR:
        switch (node->label.target_type)
        {
        case TOK_REGPC:
            asmvars_add(node->label.ident, pc);
            break;
        case TOK_IDENT:
            asmvars_add(node->label.ident, node->label.target->op.evaluate(node->label.target));
            break;
        }
        break;
    default:
        throw_error(E_LINKERERROR, "\n Unknown NodeType\n");
        break;
    }
    if(bkasm_stage == GENERATE_STAGE && size != 0 && node->type == NODE_INSTRUCTION )
        codegen_generate(node, pc, size);
    return size;
}

unsigned char* codegen_link(ASTree* ast)
{
    int pc = 0;
    int instrSize;
    NodeList *it;

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
    return prog;
}
