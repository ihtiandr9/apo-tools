#include <stdio.h>
#include <string.h>
#include "bkasm.h"
#include "asmvars.h"
#include "errors.h"
#include "hashtable.h"

HashVar asmvars[VAR_COUNT];

void asmvars_add(const char key[], int val)
{

    if (key == 0 || strlen(key) == 0)
    {
        fprintf(stderr, "INTERNAL_ERROR: asmvars_add: key is null");
        return;
    }
    hash_push(key, val, asmvars, VAR_COUNT);
}

int asmvars_value(const char key[], int *result)
{
    if (key == 0 || strlen(key) == 0)
    {
        fprintf(stderr, "INTERNAL_ERROR: asmvars_value: key is null");
        return -1;
    }
    else
    {
            return hash_value(key, result, asmvars, VAR_COUNT);
        }
}