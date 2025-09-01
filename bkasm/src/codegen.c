#include <stdio.h>
#include "bkasm.h"
#include "codegen.h"
#include "asmast.h"
#include "asmvars.h"
#include "errors.h"
#include "lexer.h"
#include "mathexpr.h"
#include "nodes.h"

char prog[MAX_PROG_SIZE];

int codegen_generate(Node *node, int pc, ASTree *ast)
{
    int size = 0;
    char err_msg[MAX_ERR_MSG_LEN];

    if (node->type == NODE_INSTRUCTION)
    {
        switch (node->op.opcode)
        {
        case TOK_ORG:
            size = node->op.lparam->op.evaluate(node->op.lparam);
            break;
        default:
            sprintf(err_msg, "Unexpected instruction:\n unknown opcode %s\n", node->op.ident);
            throw_error(E_LINKERERROR, err_msg);
            break;
        }
    }
    return size;
}

int codegen_evaluate_params(Node *node, int pc, ASTree *ast)
{
    char err_msg[MAX_ERR_MSG_LEN];
    int size = 0;
    switch (node->type)
    {
    case NODE_INSTRUCTION:
        switch (node->op.opcode)
        {
        case TOK_CALL:
        case TOK_JMP:
        case TOK_JZ:
            size = 3;
            node->op.lparam->op.evaluate(node->op.lparam);
            break;
        case TOK_ORG:
            size = node->op.lparam->op.evaluate(node->op.lparam);
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
    return size;
}

char* codegen_link(ASTree* ast)
{
    int pc = 0;
    int instrSize;
    NodeList *it;

    for(bkasm_stage = EVAL_STAGE; bkasm_stage <= GENERATE_STAGE; bkasm_stage ++)
    {
        for(it = ast->firstNode; it; it = it -> next)
        {
            if (bkasm_stage == GENERATE_STAGE)
            {
                instrSize = codegen_generate(&it->node, pc, ast);
                printf("DEBUG: instruction size: %d pc: %d\n", instrSize, pc); //FIXME remove
            }
            else
            {
                instrSize = codegen_evaluate_params(&it->node, pc, ast);
            }
            pc += instrSize;
        }
        pc = 0;
    }
    asmvars_print();
    return prog;
}

