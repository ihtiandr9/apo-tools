#include <globals.h>
#include <errors.h>
#include <stdio.h>

static  fn_error_dispatecher error_dispatcher = 0;

static void default_error_dispatcher(Error *e)
{
    printf("Error: ");
    switch (e->type)
    {
    case E_UNKIDENT:
        printf("Unknown identifier: %s\n", (char *)e->data);
        break;
    case E_UNEXPSYM:
        printf("Unexpected symbol: %c (may be in lowercase)\n", *(char*)e->data);
        break;
    case E_UNEXPTOKEN:
        printf("Unexpected token: %s\n", (char*)e->data);
        break;
    case E_UNKKEYWORD:
        printf("Unexpected keyword: %s\n", (char*)e->data);
        break;
    default:
        printf("Unknown error\n");
        exit_nicely();
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