#ifndef H_BKINTS_H
#define H_BKINTS_H
#include <globals.h>

STRUCT(IntBase)
{
    char suffix;
    int radix;
};
int radix(char m_ch);
#endif