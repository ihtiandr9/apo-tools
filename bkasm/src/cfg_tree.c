
#include <unistd.h>
#include <stdlib.h>
#include <parser.h>
#include <cfg_tree.h>

void cfg_tree_add_statement(Parser* parser, Program *program)
{
    Node *m_statement = (Node *)parser->statement;
    parser->statement = 0;
    if (!m_statement)
        return;
    if (NODE_INSTRUCTION != m_statement->type)
        return;
    Instruction m_instruction = *(Instruction *)m_statement;
    free(m_statement);
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
    program->last->instruction = m_instruction;
}
