#include <bkasm.h>
#include <errors.h>
#include <stdio.h>

static  fn_error_dispatcher error_dispatcher = 0;

static void default_error_dispatcher(Error *e)
{
    fprintf(stderr, "Error: ");
    switch (e->type)
    {
    case E_UNKIDENT:
        fprintf(stderr, "Unknown identifier: %s\n", (char *)e->data);
        break;
    case E_UNEXPSYM:
        fprintf(stderr, "Unexpected symbol: \"%c\" (may be in lowercase)\n", *(char*)e->data);
        break;
    case E_UNEXPTOKEN:
        fprintf(stderr, "Unexpected token: %s\n", (char*)e->data);
        break;
    case E_UNKKEYWORD:
        fprintf(stderr, "Unexpected keyword: %s\n", (char*)e->data);
        break;
    case E_SYNTAXERROR:
        fprintf(stderr, "Syntax error in expression: %s\n", (char*)e->data);
        break;
    default:
        fprintf(stderr, "Unknown error\n");
        exit_nicely(-1);
        break;
    }
}

void throw_error(eErrorType type, void *data)
{
    Error e;
    e.data = data;
    e.type = type;
    if (!error_dispatcher)
        default_error_dispatcher(&e);
    else
        error_dispatcher(&e);
}

void set_error_dipatcher(void(*_error_dispatcher)(Error*))
{
    error_dispatcher = _error_dispatcher;
}

