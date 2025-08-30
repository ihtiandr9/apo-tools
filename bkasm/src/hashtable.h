#ifndef H_HASHTABLE_H
#define H_HASHTABLE_H

STRUCT(HashVar)
{
    char name[50];
    int val;
};
void hash_push(const char key[], int val, HashVar table[], int size);
int hash_value(const char key[], int *result, HashVar table[], int size);
void hash_print(HashVar table[]);
void hash_clear(HashVar table[]);
void hash_init(HashVar table[]);

#endif
