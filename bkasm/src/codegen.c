#include <bkasm.h>
#include <mathexpr.h>
#include <asmast.h>
#include <codegen.h>

char prog[MAX_PROG_SIZE];

int codegen_generate(Node* node)
{
    int size = 0;
    printf("%s type instruction\n", node->label.ident);
    return size;
}

int codegen_evaluate_params(Node* node){
    int size = 0;
    printf("%s type instruction\n", node->label.ident);
    return size;

}

char* codegen_link(ASTree* ast)
{
    int pc = 0;
    int instrSize;
    CodegenStage stage;
    NodeList *it;

    for(stage = EVAL_STAGE; stage <= GENERATE_STAGE; stage ++) {
        for(it = ast->firstNode; it; it = it -> next){
            switch (it->node.type) {
                case NODE_INSTRUCTION:
                    if(stage == GENERATE_STAGE){
                        instrSize = codegen_generate(&it->node);
                    } else{
                        codegen_evaluate_params(&it->node);
                    }

                    break;
                case NODE_LABEL:
                    printf("%s type label\n", it->node.op.ident);
                    break;
                default:
                    fprintf(stderr, "Unknown NodeType");
            }
        }
    }
    return prog;
}

