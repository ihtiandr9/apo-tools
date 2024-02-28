#ifndef H_ERRORS_H
#define H_ERRORS_H

typedef enum _errType
{
    E_UNKIDENT,
    E_UNEXPSYM,
    E_UNEXPTOKEN,
    E_UNKKEYWORD,
} ErrorType;

typedef struct _TError
{
    ErrorType type;
    void *data;
} Error;

typedef void (*fn_error_dispatecher)(Error *);

void throw_error(ErrorType etype, void *data);

void set_error_dipatcher(void(*_error_dipatcher)(Error*));

#endif
