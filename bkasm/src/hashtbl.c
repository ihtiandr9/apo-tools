#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtbl.h"

unsigned int hashf(const char key[])
{
    unsigned int hash = 0;
    for (const char *it = key; *it; it++)
        hash = hash * 31 + (unsigned char)*it;
    return hash;
}

void hash_push(const char key[], int val, HashVar table[], unsigned int size)
{
    unsigned int i;
    unsigned int index = hashf(key) % size;

    for (i = 0; i < size; i++)
    {
        signed char ch = table[index].name[0];
        if (ch == 0 || ch == -1 || strcmp(key, table[index].name) == 0)
        {
            strncpy(table[index].name, key, 49);
            table[index].name[49] = '\0';
            table[index].val = val;
            return;
        }
        index = (index + 1) % size;
    }
}

int hash_value(const char key[], int *result, HashVar table[], unsigned int size)
{
    unsigned int i;
    unsigned int index = hashf(key) % size;

    for (i = 0; i < size; i++)
    {
        signed char ch = table[index].name[0];
        if (ch == 0)
            return -2;
        if (ch != -1 && strcmp(key, table[index].name) == 0)
        {
            if (result)
                *result = table[index].val;
            return 0;
        }
        index = (index + 1) % size;
    }

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
    free(sorted);

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

const char* hash_text_error(int errno)
{
    static const char* errs[] = {
        "ERROR: internal error\n null pointer to return value\n",
        "ERROR: internal error\n key not found in table\n"
    };
    static const char* unknown = "ERROR: unknown error\n";

    if (errno >= -2 && errno < 0) {
        return errs[-errno - 1];
    }
    return unknown;  /* Никогда не возвращаем NULL */
}

