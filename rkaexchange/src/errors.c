#include <globals.h>
#include <errors.h>
#include <stdio.h>

static  fn_error_dispatecher error_dispatcher = 0;

static void default_error_dispatcher(Error *e)
{
    fprintf(stderr,"Error: ");
    switch (e->type)
    {
    case E_CMDREQ:
        fprintf(stderr, "Command \"pack\" or \"unpack\" required\n");
        exit_nicely(E_CMDREQ);
        break;
    case E_UNKNOWNCMD:
        fprintf(stderr, "Unknown command: %s\n", (char*)e->data);
        exit_nicely(E_UNKNOWNCMD);
        break;
    case E_CREATE:
        fprintf(stderr, "Can't create output file: %s\n", (char*)e->data);
        exit_nicely(E_CREATE);        
        break;
    case E_FMT:
        fprintf(stderr, "Error of file format: %s\n", (char*)e->data);
        break;
    case E_NOEXIST:
        fprintf(stderr, "File not exist: %s\n", (char*)e->data);
        exit_nicely(E_NOEXIST);
        break;
    case E_UNDEFERR:
    default:
        fprintf(stderr, "Unknown error\n");
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

