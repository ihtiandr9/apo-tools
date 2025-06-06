#include <bkasm.h>
#include <bkints.h>

static const IntBase intBases[] =
{
    {'H', 16},
    {'O', 8},
    {'B', 2},
    {'\0', 10},
};

int radix(char suffix)
{
    IntBase *intBase = (IntBase *)intBases;
    while (intBase->suffix != 0)
    {
        if (intBase->suffix == suffix)
            break;
        intBase++;
    }
    return intBase->radix;
}
