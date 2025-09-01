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

void codegen_generate(Node *node, int pc, int size)
{
    char err_msg[MAX_ERR_MSG_LEN];

    if (node->type == NODE_INSTRUCTION)
    {
        if(size)
        {
            prog[pc] = node->op.opcode;
            prog[pc + 1] = node->op.lparam->op.evaluate(node->op.lparam);
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
        case TOK_CALL: // 2 bytes one param instructions
        case TOK_JMP:
        case TOK_JZ:
            size = 3;
            node->op.lparam->op.evaluate(node->op.lparam);
            break;
        case TOK_DB:
            size = 1;
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
    if(bkasm_stage == GENERATE_STAGE && size != 0 && node->type == NODE_INSTRUCTION )
        codegen_generate(node, pc, size);
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
            instrSize = codegen_evaluate_ast(&it->node, pc, ast);
            printf("DEBUG: instruction size: %d pc: %d\n", instrSize, pc); // FIXME remove
            pc += instrSize;
        }
        pc = 0;
    }
    asmvars_print();
    return prog;
}
