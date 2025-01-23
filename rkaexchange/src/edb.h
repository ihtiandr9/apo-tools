
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
//#ifndef uint8_t
//typedef unsigned char uint8_t;
//#endif

wchar_t getUnicodeSymbol(uint8_t chr);


typedef struct t_edb
{
    FILE *fd_in;
    FILE *fd_out;
    int (*unpack)(struct t_edb *this);
    int (*pack)(struct t_edb *this);     
} Edb, *ptrEdb;

ptrEdb edb_create(FILE *fd_in, FILE *fd_out);
