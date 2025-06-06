#ifndef H_BKINTS_H
#define H_BKINTS_H
#include <bkasm.h>

STRUCT(IntBase)
{
    char suffix;
    int radix;
};
int radix(char suffix);
#endif
