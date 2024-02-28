#ifndef H_ERRORS_H
#define H_ERRORS_H

typedef enum _eErrorType
{
    E_UNKIDENT,
    E_UNEXPSYM,
    E_UNEXPTOKEN,
    E_UNKKEYWORD,
} eErrorType;

typedef struct _Error
{
    eErrorType type;
    void *data;
} Error;

typedef void (*fn_error_dispatecher)(Error *);

void throw_error(eErrorType etype, void *data);

void set_error_dipatcher(void(*_error_dipatcher)(Error*));

#endif
