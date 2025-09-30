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
#include "bkasm.h"
#include "asmast.h"
#include "asmvars.h"
#include "mathexpr.h"
#include "nodes.h"

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
        break;
    case NODE_VAR:
        switch (statement->u.label.target_type)
        {
        case TOK_REGPC: // unresolved label postponed to linker
        case TOK_IDENT: // variable declaration
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
        
            asmvars_add(statement->ident, statement->u.label.target->op.evaluate(statement->u.label.target));
            break;

        default:
            break;
        }
        break;
    default:
        assert(0);
        return;
    }
    // node_print(statement);
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
    free(astree);
}
