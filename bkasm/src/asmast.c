#include <bkasm.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#ifndef WIN32
#ifndef __MSDOS__
#ifndef _Windows
#include <unistd.h>
#endif
#endif
#endif
#include <asmast.h>

void ast_add_statement(Node *statement, ASTree *astree)
{
    if (!statement)
        return;
    switch (statement->type)
    {
        case NODE_INSTRUCTION:
            if (astree->lastNode)
            {
                astree->lastNode->next = nodelist_alloc();
                astree->lastNode = astree->lastNode->next;
            }
            else
            {
                astree->firstNode = nodelist_alloc();
                astree->lastNode = astree->firstNode;
            }
            astree->lastNode->node = *statement;
            printf(INDENT "< OPERATION >: %s code %d\n", astree->lastNode->node.op.ident,
                    astree->lastNode->node.op.opcode);
            break;
        case NODE_LABEL:
            if (astree->lastNode)
            {
                astree->lastNode->next = nodelist_alloc();
                astree->lastNode = astree->lastNode->next;
            }
            else
            {
                astree->firstNode = nodelist_alloc();
                astree->lastNode = astree->firstNode;
            }
            astree->lastNode->node = *statement;
            printf("< LABEL >: %s\n", astree->lastNode->node.label.ident);
            break;
        default:
            assert(0);
            return;
    }
    free(statement);
}

ASTree *ast_create()
{
    ASTree *astree = (ASTree *)malloc(sizeof(ASTree));
    ast_init(astree);
    return astree;
}

void ast_init(ASTree* astree)
{
    astree->firstNode = NULL;
    astree->lastNode = NULL;
}

void ast_free(ASTree *astree)
{
    if (astree)
    {
        ast_destroy(astree);
        free(astree);
    }
}

void ast_destroy(ASTree *astree)
{
    if (astree)
    {
        if (astree->firstNode)
        {
            nodelist_destroy(astree->firstNode);
        }
        astree->firstNode = 0;
        astree->lastNode = 0;
    }
}
