#include <stdio.h>
#include <string.h>
#include "bkasm.h"
#include "errors.h"
#include "hashtable.h"

unsigned int hashf(const char key[])
{
    char *it;
    unsigned char hash = 0;
    for (it = (char*) key; *it; it++)
        hash = hash * 3 + *it;
    return hash;
}

void hash_push(const char key[], int val, HashVar table[], int size)
{
    unsigned int index = hashf(key) % size;
    for(char ch = table[index].name[0]; ch != 0 && ch != -1 && index < size; ++index)
        ch = table[index].name[0];
    strcpy(table[index].name, key);
    table[index].val = val;
}

int hash_value(const char key[], int *result, HashVar table[], int size)
{
    char ch, errmsg[100];
    unsigned char index = hashf(key);
    for (ch = table[index].name[0]; index < size && ( ch == -1 || strcmp(key, table[index].name)); ++index)
    {
        if (ch == 0)
        {
            sprintf(errmsg, "\nUndefined variable %s\n", key);
            throw_error(E_SYNTAXERROR, errmsg);
            return -1;
        }
        ch = table[index].name[0];
    }
    if (result)
    {
        *result = table[index].val;
        return 0;
    }
    else
    {
        fprintf(stderr, "ERROR: internal error\n");
        fprintf(stderr, "null pointer to return value\n");
        return -1;
    }
}