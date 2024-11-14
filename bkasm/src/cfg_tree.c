#include <globals.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <cfg_tree.h>
#include <parser.h>

CFG_Tree *cfg_tree_create()
{
    CFG_Tree *tree;
    tree = (CFG_Tree *)malloc(sizeof(CFG_Tree));
    tree->next = 0;
    tree->node.type = NODE_EMPTY;
    return tree;
}

void cfg_tree_add_statement(Parser *parser, Program *program)
{
    Node *m_statement = (Node *)parser->statement;
    parser->statement = 0;
    if (!m_statement)
        return;
    switch (m_statement->type)
    {
    case NODE_INSTRUCTION:
        if (program->last)
        {
            program->last->next = cfg_tree_create();
            program->last = program->last->next;
        }
        else
        {
            program->first = cfg_tree_create();
            program->last = program->first;
        }
        program->last->node = *m_statement;
        free(m_statement);
        m_statement = 0;
        printf(INDENT "< OPERATION >: %s code %d\n", program->last->node.op.ident,
               program->last->node.op.opcode);
        break;
    case NODE_LABEL:
        if (program->last)
        {
            program->last->next = cfg_tree_create();
            program->last = program->last->next;
        }
        else
        {
            program->first = cfg_tree_create();
            program->last = program->first;
        }
        program->last->node = *m_statement;
        free(m_statement);
        m_statement = 0;
        printf("< LABEL >: %s\n", program->last->node.label.ident);
        break;
    default:
        assert(0);
        return;
    }
    assert(!m_statement);
}

Program *program_create()
{
    Program *prg = (Program *)malloc(sizeof(Program));
    program_init(prg);
    return prg;
}

void program_init(Program* prg)
{
    prg->first = NULL;
    prg->last = NULL;
}

void program_free(Program *prg)
{
    if (prg)
    {
        program_destroy(prg);
        free(prg);
    }
}

void program_destroy(Program *prg)
{
    if (prg)
    {
        if (prg->first)
        {
            destroyCFGTree(prg->first);
        }
        prg->first = 0;
        prg->last = 0;
    }
}

void destroyCFGTree(CFG_Tree *tree)
{
    CFG_Tree *next = 0;
    while (tree)
    {
        next = tree->next;
        clearNode(&tree->node);
        free(tree);
        tree = next;
    }
}