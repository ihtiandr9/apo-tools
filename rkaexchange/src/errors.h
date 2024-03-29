#ifndef H_ERRORS_H
#define H_ERRORS_H

typedef enum _eErrorType
{
    E_CMDREQ,
    E_UNKNOWNCMD,
    E_NOEXIST,
    E_CREATE,
    E_FMT,
        E_UNDEFERR	
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
