#include <stdio.h>
#include "bkasm.h"
#include "errors.h"
#include "clog.h"

static void default_error_dispatcher(Error *e)
{
    switch (e->type)
    {
    case E_UNKIDENT:
        ERROR("Unknown identifier: %s", (char *)e->data);
        break;
    case E_UNEXPSYM:
        ERROR("Unexpected symbol: \"%c\" (may be in lowercase)", *(char*)e->data);
        break;
    case E_UNEXPTOKEN:
        ERROR("Unexpected token: %s", (char*)e->data);
        break;
    case E_UNKKEYWORD:
        ERROR("Unexpected keyword: %s", (char*)e->data);
        break;
    case E_SYNTAXERROR:
        ERROR("Syntax error in expression: %s", (char*)e->data);
        break;
    case E_LINKERERROR:
        ERROR("Linker: %s", (char*)e->data);
        break;
    case E_INTERNALERROR:
        ERROR("Internal error: %s", (char*)e->data);
        break;
    default:
        ERROR("Unknown error");
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
