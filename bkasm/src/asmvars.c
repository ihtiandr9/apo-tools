#include <stdio.h>
#include <string.h>
#include "bkasm.h"
#include "asmvars.h"
#include "errors.h"
#include "hashtable.h"

HashVar asmvars[MAX_VAR_COUNT];

void asmvars_add(const char key[], int val)
{

    if (key == 0 || strlen(key) == 0)
    {
        fprintf(stderr, "INTERNAL_ERROR: asmvars_add: key is null");
        return;
    }
    hash_push(key, val, asmvars, MAX_VAR_COUNT);
}

int asmvars_value(const char key[], int *result)
{
    int hash_result =0;
    char err_msg[MAX_ERR_MSG_LEN];
    if (key == 0 || strlen(key) == 0)
    {
        fprintf(stderr, "INTERNAL_ERROR: asmvars_value: key is null");
        hash_result = -1;
    }
    else
    {
        hash_result = hash_value(key, result, asmvars, MAX_VAR_COUNT);
        if (hash_result == -1 && bkasm_stage > PARSE_STAGE)
        {
            sprintf(err_msg, "\nUndefined variable %s\n", key);
            throw_error(E_SYNTAXERROR, err_msg);
        }
    }
    return hash_result;
}

void asmvars_print()
{
    hash_print(asmvars);
}
