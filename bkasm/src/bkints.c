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
    int i;
    for (i=0; intBases[i].suffix; )
    {
        if (intBases[i].suffix == suffix)
            break;
        i++;
    }
    return intBases[i].radix;
}
