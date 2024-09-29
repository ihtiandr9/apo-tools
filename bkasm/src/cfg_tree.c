#include <globals.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <cfg_tree.h>
#include <parser.h>

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
            program->last->next = (CFG_Tree *)malloc(sizeof(CFG_Tree));
            program->last = program->last->next;
        }
        else
        {
            program->first = (CFG_Tree *)malloc(sizeof(CFG_Tree));
            program->last = program->first;
        }
        program->last->node = *m_statement;
        free(m_statement);
        printf("    < OPERATION >: %s code %d\n", program->last->node.op.ident,
            program->last->node.op.opcode);

        break;
    case NODE_LABEL:
        if (program->last)
        {
            program->last->next = (CFG_Tree *)malloc(sizeof(CFG_Tree));
            program->last = program->last->next;
        }
        else
        {
            program->first = (CFG_Tree *)malloc(sizeof(CFG_Tree));
            program->last = program->first;
        }
        program->last->node = *m_statement;
        free(m_statement);
        printf("< LABEL >: %s\n", program->last->node.label.ident);
        break;
    default:
        assert(0);
        return;
    }
}

void destroyProgram(Program* prg)
{
    CFG_Tree *tmp;
    CFG_Tree *next;
    for(tmp = prg->first; tmp; tmp = next)
    {
        next = tmp->next;
        free(tmp->node.op.ident);
        free(tmp);
    }
}


