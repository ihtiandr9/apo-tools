#include <stdio.h>
#include "bkasm.h"
#include "codegen.h"
#include "asmast.h"
#include "lexer.h"
#include "mathexpr.h"
#include "nodes.h"

char prog[MAX_PROG_SIZE];

void codegen_print_labels(){
  /*
   NodeList* var;
    printf("GEN: labels\n");
    for (var = ast -> vars; var; var = var->next)
    {
        printf("DEBUG: %s type label: %d\n", var->node.label.ident, var->node.label.target->op.evaluate(var->node.label.target));
    }
 */
}

int codegen_generate(Node *node, int pc, ASTree *ast)
{
    int size = 0;
    if (node->type == NODE_INSTRUCTION)
    {
        switch (node->op.opcode)
        {
        case TOK_ORG:
            size = node->op.lparam->op.evaluate(node->op.lparam);
            break;
        default:
            fprintf(stderr, "ERROR: unknown token %s\n", node->label.ident);
            break;
        }
    }
    return size;
}

int codegen_evaluate_params(Node *node, int pc, ASTree *ast)
{
    int size = 0;
    switch (node->type){
        case NODE_INSTRUCTION:
            switch (node->op.opcode)
            {
                case TOK_ORG:
                    size = node->op.lparam->op.evaluate(node->op.lparam);
                    break;
                default:
                    fprintf(stderr, "ERROR: unknown token %s\n", node->op.ident);
                    break;
            }
            break;
        case NODE_VAR:
            if (node->label.target_type = TOK_REGPC)
            {
                Node *label = node_create_label(node->label.ident);
                label->label.target_type = TOK_IDENT;
                label->label.target = const_create(pc);
                ast_add_statement(label, ast);
            }
            
            // printf("DEBUG: %s type label\n", node->label.ident); //FIXME implement
            break;
        default:
            fprintf(stderr, "Unknown NodeType");
    }
    return size;
}

char* codegen_link(ASTree* ast)
{
    int pc = 0;
    int instrSize;
    NodeList *it;

    for(bkasm_stage = EVAL_STAGE; bkasm_stage <= GENERATE_STAGE; bkasm_stage ++) {
        for(it = ast->firstNode; it; it = it -> next){
                if (bkasm_stage == GENERATE_STAGE)
                {
                    instrSize = codegen_generate(&it->node, pc, ast);
                }
                else
                {
                    instrSize = codegen_evaluate_params(&it->node, pc, ast);
                }
                pc += instrSize;
            printf("DEBUG: instruction size: %d pc: %d\n", instrSize, pc); //FIXME remove
        }
        pc = 0;
    }
    codegen_print_labels(ast);
    return prog;
}

