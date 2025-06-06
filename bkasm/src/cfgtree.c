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
#include <cfgtree.h>

NodeList *cfg_tree_create()
{
    NodeList *nodeList;
    nodeList = (NodeList *)malloc(sizeof(NodeList));
    nodeList->next = 0;
    nodeList->node.type = NODE_EMPTY;
    return nodeList;
}

void cfg_tree_add_statement(Node *statement, CfgTree *cfgtree)
{
    if (!statement)
        return;
    switch (statement->type)
    {
    case NODE_INSTRUCTION:
        if (cfgtree->lastNode)
        {
            cfgtree->lastNode->next = cfg_tree_create();
            cfgtree->lastNode = cfgtree->lastNode->next;
        }
        else
        {
            cfgtree->firstNode = cfg_tree_create();
            cfgtree->lastNode = cfgtree->firstNode;
        }
        cfgtree->lastNode->node = *statement;
        printf(INDENT "< OPERATION >: %s code %d\n", cfgtree->lastNode->node.op.ident,
               cfgtree->lastNode->node.op.opcode);
        break;
    case NODE_LABEL:
        if (cfgtree->lastNode)
        {
            cfgtree->lastNode->next = cfg_tree_create();
            cfgtree->lastNode = cfgtree->lastNode->next;
        }
        else
        {
            cfgtree->firstNode = cfg_tree_create();
            cfgtree->lastNode = cfgtree->firstNode;
        }
        cfgtree->lastNode->node = *statement;
        printf("< LABEL >: %s\n", cfgtree->lastNode->node.label.ident);
        break;
    default:
        assert(0);
        return;
    }
    free(statement);
}

CfgTree *cfgtree_create()
{
    CfgTree *cfgtree = (CfgTree *)malloc(sizeof(CfgTree));
    cfgtree_init(cfgtree);
    return cfgtree;
}

void cfgtree_init(CfgTree* cfgtree)
{
    cfgtree->firstNode = NULL;
    cfgtree->lastNode = NULL;
}

void cfgtree_free(CfgTree *cfgtree)
{
    if (cfgtree)
    {
        cfgtree_destroy(cfgtree);
        free(cfgtree);
    }
}

void cfgtree_destroy(CfgTree *cfgtree)
{
    if (cfgtree)
    {
        if (cfgtree->firstNode)
        {
            nodelist_destroy(cfgtree->firstNode);
        }
        cfgtree->firstNode = 0;
        cfgtree->lastNode = 0;
    }
}
