#ifndef H_ERRORS_H
#define H_ERRORS_H

typedef enum _eErrorType
{
    E_SUCCESS,
    E_UNKIDENT,
    E_UNEXPSYM,
    E_UNEXPTOKEN,
    E_UNKKEYWORD,
    E_SYNTAXERROR,
    E_LINKERERROR,
    E_INTERNALERROR,
} eErrorType;

STRUCT(Error)
{
    eErrorType type;
    void *data;
};

typedef void (*fn_error_dispatcher)(Error *);

void throw_error(eErrorType etype, void *data);
void throw_error_disp(eErrorType type, void *data, fn_error_dispatcher dispatcher);

#endif
