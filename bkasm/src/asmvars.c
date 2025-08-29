#include <stdio.h>
#include <string.h>
#include <errors.h>
#include <asmvars.h>

struct var
{
    char name[50];
    int val;
};

struct var hash_vars[256];

unsigned char hashf(const char key[])
{
    char *it;
    unsigned char hash = 0;
    for (it = (char*) key; *it; it++)
        hash = hash * 3 + *it;
    return hash;
}

void hash_push(const char key[], int val)
{
    unsigned char index = hashf(key);
    for(char ch = hash_vars[index].name[0]; ch != 0 && ch != -1; ++index)
        ch = hash_vars[index].name[0];
    strcpy(hash_vars[index].name, key);
    hash_vars[index].val = val;
}

int hash_value(const char key[], int *result)
{
    char ch, errmsg[100];
    unsigned char index = hashf(key);
    for (ch = hash_vars[index].name[0]; index < 255 && ( ch == -1 || strcmp(key, hash_vars[index].name)); ++index)
    {
        if (ch == 0)
        {
            sprintf(errmsg, "\nUndefined variable %s\n", key);
            throw_error(E_SYNTAXERROR, errmsg);
            return -1;
        }
        ch = hash_vars[index].name[0];
    }
    if (result)
    {
        *result = hash_vars[index].val;
        return 0;
    }
    else
    {
        fprintf(stderr, "ERROR: internal error\n");
        fprintf(stderr, "null pointer to return value\n");
        return -1;
    }
}
