#ifndef H_ERRORS_H
#define H_ERRORS_H

#include <globals.h>

typedef enum _eErrorType
{
    E_UNKIDENT,
    E_UNEXPSYM,
    E_UNEXPTOKEN,
    E_UNKKEYWORD,
    E_SYNTAXERROR
} eErrorType;

STRUCT(Error)
{
    eErrorType type;
    void *data;
};

typedef void (*fn_error_dispatcher)(Error *);

void throw_error(eErrorType etype, void *data);

void set_error_dipatcher(void(*_error_dipatcher)(Error*));

#endif
