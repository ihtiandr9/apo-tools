#include <globals.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#ifndef WIN32
#include <unistd.h>
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

void cfg_tree_add_statement(Node *statement, Gfg_Tree *cfgtree)
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
}

Gfg_Tree *cfgtree_create()
{
    Gfg_Tree *cfgtree = (Gfg_Tree *)malloc(sizeof(Gfg_Tree));
    cfgtree_init(cfgtree);
    return cfgtree;
}

void cfgtree_init(Gfg_Tree* cfgtree)
{
    cfgtree->first = NULL;
    cfgtree->last = NULL;
}

void cfgtree_free(Gfg_Tree *cfgtree)
{
    if (cfgtree)
    {
        cfgtree_destroy(cfgtree);
        free(cfgtree);
    }
}

void cfgtree_destroy(Gfg_Tree *cfgtree)
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
