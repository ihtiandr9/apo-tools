#include <stdio.h>

// statics
const unsigned char eof_sym = 0xFF;
const unsigned char eol_sym = 0x0A;
static const char decimals[] = "0123456789";
static const char digits[] = "0123456789ABCDEF";
static const char alfas[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int is_decimal(char ch)
{
    int i = 0;
    for (i = 0; i < sizeof(decimals) - 1; i++)
    {
        if (ch == decimals[i])
            break;
    }
    return (ch == decimals[i]);
}

int is_digit(char ch)
{
    int i = 0;
    for (i = 0; i < sizeof(digits) - 1; i++)
    {
        if (ch == digits[i])
            break;
    }
    return (ch == digits[i]);
}

int is_alfa(char ch)
{
    int i = 0;
    
	for (i = 0; i < sizeof(alfas) - 1; i++)
    {
        if (ch == alfas[i])
            break;
    }
    return (ch == alfas[i]);
}
