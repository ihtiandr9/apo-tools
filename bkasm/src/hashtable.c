#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

unsigned int hashf(const char key[])
{
    char *it;
    unsigned char hash = 0;
    for (it = (char*) key; *it; it++)
        hash = hash * 3 + *it;
    return hash;
}

void hash_push(const char key[], int val, HashVar table[], unsigned int size)
{
    unsigned int index = hashf(key) % size;
	signed char ch;

    for(ch = table[index].name[0]; ch != 0 && ch != -1 && index < size && strcmp(key, table[index].name); ++index)
        ch = table[index].name[0];
    strcpy(table[index].name, key);
    table[index].val = val;
}

int hash_value(const char key[], int *result, HashVar table[], unsigned int size)
{
    signed char ch;
    unsigned char index = hashf(key);

    for (ch = table[index].name[0]; index < size && (ch == -1 || strcmp(key, table[index].name)); ++index)
    {
        if (ch == 0)
            return -2;
        ch = table[index].name[0];
    }
    if (result)
    {
        *result = table[index].val;
        return 0;
    }
    else
        return -1;
}

static int hash_name_cmp(const void *p1, const void *p2)
{
    /* The actual arguments to this function are "pointers to
    pointers to HashVar* , but strcmp(3) arguments are "pointers
    to char", hence the following cast plus dereference. */
    const HashVar* var1 = *(const HashVar **) p1;
    const HashVar* var2 = *(const HashVar **) p2;

    return strcmp(var1->name, var2->name);
}

void hash_printsorted(HashVar table[], unsigned int size)
{
	unsigned int i;
	unsigned int sorted_size = 0;
    HashVar **sorted = (HashVar**) malloc(size * sizeof(HashVar*));
    
    for (i = 0; i < size; i++)
    {
        if (table[i].name[0] != 0)
        {
            sorted[sorted_size++] = &table[i];
        }
    }
    
    qsort(sorted, sorted_size, sizeof(HashVar*), hash_name_cmp);
    for (i = 0; i < sorted_size; i++)
    {
        printf("%s = %d\n", sorted[i] -> name,
            sorted[i] -> val);
    }
}

void hash_print(HashVar table[], unsigned int size)
{
	unsigned int i;
    for (i = 0; i < size; i++)
    {
        if (table[i].name[0] != 0)
        {
            printf("%s = %d\n", table[i].name, table[i].val);
        }
    }
}

const char* hash_text_error(int err)
{
    static const char* errs[] = {
        "ERROR: internal error\n null pointer to return value\n",
        "ERROR: internal error\n key not found in table\n"
    };
    static const char* unknown = "ERROR: unknown error\n";

    if (err >= -2 && err < 0) {
        return errs[-err - 1];
    }
    return unknown;  // Никогда не возвращаем NULL
}

