#include <stdio.h>

// statics
const char eof_sym = 0xFF;
const char eol_sym = 0x0A;
static const char digits[] = "0123456789";
static const char alfas[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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
    // printf("Size of alfa: %d ", sizeof(alfa));
    for (i = 0; i < sizeof(alfas) - 1; i++)
    {
        if (ch == alfas[i])
            break;
    }
    return (ch == alfas[i]);
}

