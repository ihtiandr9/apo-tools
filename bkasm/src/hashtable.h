#ifndef H_HASHTABLE_H
#define H_HASHTABLE_H

typedef struct
{
    char name[50];
    int val;
} HashVar;
void hash_push(const char key[], int val, HashVar table[], unsigned int size);
int hash_value(const char key[], int *result, HashVar table[], unsigned int size);
void hash_print(HashVar table[], unsigned int size);
void hash_clear(HashVar table[]);
void hash_init(HashVar table[]);
const char* hash_text_error(int errno);

#endif
