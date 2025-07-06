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
    const IntBase* base = intBases;
    for (base = intBases; base -> suffix; base++)
    {
        if (base -> suffix == suffix)
            break;
    }
    return base -> radix;
}
