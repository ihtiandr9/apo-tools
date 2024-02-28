
#include <unistd.h>
#include <stdlib.h>
#include <parser.h>
#include <program.h>

static Program program, *last = 0;

void program_generate(pParser parser)
{
    Node *m_statement = parser->statement;
    parser->statement = 0;
    if (!m_statement)
        return;
    if (OP_INSTRUCTION != m_statement->type)
        return;
    Operation m_operation = *(Operation*)m_statement;
    free(parser->statement);
    if (last)
    {
        last->next = (Program *)malloc(sizeof(Program));
        last = last->next;
    }
    else
        last = &program;
    last->operation =m_operation;
}
