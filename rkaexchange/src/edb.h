
#include<stddef.h>
#include<stdint.h>
wchar_t getUnicodeSymbol(uint8_t chr);


typedef struct t_edb
{
    int fd_in;
    int fd_out;
    int (*unpack)(struct t_edb *this);
    int (*pack)(struct t_edb *this);     
} Edb, *ptrEdb;

ptrEdb edb_create(int fd_in, int fd_out);