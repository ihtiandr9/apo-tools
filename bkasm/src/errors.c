#include <stdio.h>
#include "bkasm.h"
#include "errors.h"

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
    case E_LINKERERROR:
        fprintf(stderr, "Linker error: %s\n", (char*)e->data);
        break;
    case E_INTERNALERROR:
        fprintf(stderr, "Internal error: %s\n", (char*)e->data);
        break;
    default:
        fprintf(stderr, "Unknown error\n");
        exit_nicely(-1);
        break;
    }
}

void throw_error(eErrorType type, void *data)
{
    throw_error_disp(type, data, NULL);
}

void throw_error_disp(eErrorType type, void *data, fn_error_dispatcher dispatcher)
{
    Error e;
    e.data = data;
    e.type = type;
    if (!dispatcher)
        default_error_dispatcher(&e);
    else
        dispatcher(&e);
}
