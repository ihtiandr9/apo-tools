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
	char ch;

    for(ch = table[index].name[0]; ch != 0 && ch != -1 && index < size && strcmp(key, table[index].name); ++index)
        ch = table[index].name[0];
    strcpy(table[index].name, key);
    table[index].val = val;
}

int hash_value(const char key[], int *result, HashVar table[], int size)
{
    char ch, err_msg[MAX_ERR_MSG_LEN];
    unsigned char index = hashf(key);

    for (ch = table[index].name[0]; index < size && ( ch == -1 || strcmp(key, table[index].name)); ++index)
    {
        if (ch == 0)
            return -1;
        ch = table[index].name[0];
    }
    if (result)
    {
        *result = table[index].val;
        return 0;
    }
    else
    {
		sprintf(err_msg, "ERROR: internal error\n %s", "null pointer to return value\n");
        throw_error(E_INTERNALERROR, err_msg);
        return -1;
    }
}

void hash_print(HashVar table[])
{
	int i;
    for (i = 0; i < MAX_VAR_COUNT; i++)
    {
        if (table[i].name[0] != 0)
        {
            printf("%s = %d\n", table[i].name, table[i].val);
        }
    }
}