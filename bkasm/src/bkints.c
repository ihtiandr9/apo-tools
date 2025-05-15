#include <bkasm.h>
#include <bkints.h>

static const IntBase bases[] =
{
    {'H', 16},
    {'O', 8},
    {'B', 2},
    {'\0', 10},
};

int radix(char suffix)
{
    IntBase *m_base = (IntBase *)bases;
    while (m_base->suffix != 0)
    {
        if (m_base->suffix == suffix)
            break;
        m_base++;
    }
    return m_base->radix;
}
