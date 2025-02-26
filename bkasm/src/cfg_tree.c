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
#include <cfg_tree.h>

NodeList *cfg_tree_create()
{
    NodeList *nodelist;
    nodelist = (NodeList *)malloc(sizeof(NodeList));
    nodelist->next = 0;
    nodelist->node.type = NODE_EMPTY;
    return nodelist;
}

void cfg_tree_add_statement(Node *statement, Cfg_Tree *cfgtree)
{
    if (!statement)
        return;
    switch (statement->type)
    {
    case NODE_INSTRUCTION:
        if (cfgtree->last)
        {
            cfgtree->last->next = cfg_tree_create();
            cfgtree->last = cfgtree->last->next;
        }
        else
        {
            cfgtree->first = cfg_tree_create();
            cfgtree->last = cfgtree->first;
        }
        cfgtree->last->node = *statement;
        printf(INDENT "< OPERATION >: %s code %d\n", cfgtree->last->node.op.ident,
               cfgtree->last->node.op.opcode);
        break;
    case NODE_LABEL:
        if (cfgtree->last)
        {
            cfgtree->last->next = cfg_tree_create();
            cfgtree->last = cfgtree->last->next;
        }
        else
        {
            cfgtree->first = cfg_tree_create();
            cfgtree->last = cfgtree->first;
        }
        cfgtree->last->node = *statement;
        printf("< LABEL >: %s\n", cfgtree->last->node.label.ident);
        break;
    default:
        assert(0);
        return;
    }
    free(statement);
}

Cfg_Tree *cfgtree_create()
{
    Cfg_Tree *cfgtree = (Cfg_Tree *)malloc(sizeof(Cfg_Tree));
    cfgtree_init(cfgtree);
    return cfgtree;
}

void cfgtree_init(Cfg_Tree* cfgtree)
{
    cfgtree->first = NULL;
    cfgtree->last = NULL;
}

void cfgtree_free(Cfg_Tree *cfgtree)
{
    if (cfgtree)
    {
        cfgtree_destroy(cfgtree);
        free(cfgtree);
    }
}

void cfgtree_destroy(Cfg_Tree *cfgtree)
{
    if (cfgtree)
    {
        if (cfgtree->first)
        {
            nodelist_destroy(cfgtree->first);
        }
        cfgtree->first = 0;
        cfgtree->last = 0;
    }
}
