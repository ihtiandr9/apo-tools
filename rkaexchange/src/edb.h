
#include <stdio.h>
#include <stdlib.h>

wchar_t getUnicodeSymbol(unsigned char chr);

typedef struct t_edb
{
    FILE *fd_in;
    FILE *fd_out;
    int (*unpack)(struct t_edb *this);
    int (*pack)(struct t_edb *this);
} Edb, *ptrEdb;

ptrEdb edb_create(FILE *fd_in, FILE *fd_out);
void edb_init(FILE *fd_in, FILE *fd_out, ptrEdb pEdb);
