
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#ifndef WIN32
#ifndef _Windows
#ifndef __MSDOS__
#include <unistd.h>
#endif
#endif
#endif
#include "bkasm.h"
#include "inbuf.h"

static const char* inbuf;

static InbufCurrentString currStr;
static int bufsize;
static int cursym;

InbufCurrentString *inbuf_currstr()
{
    return (InbufCurrentString *) &currStr;
}

static void inbuf_markcurrstr()
{
    int i;
    for (i = 0; i + cursym < bufsize && i < INBUF_STRING_SIZE; i++)
    {
        currStr.str[i] = inbuf[cursym + i];
        if (currStr.str[i] == 10)
            currStr.str[i] = 0;
    }
}

unsigned char inbuf_next_char()
{
    unsigned char chr;
    if (cursym < bufsize)
        chr = inbuf[cursym++];
    else
    {
        chr = 0xff;
    }
    if (chr == 10)
    {
        currStr.num++;
        inbuf_markcurrstr();
    }
    return chr;
}

void inbuf_init(const char* _inbuf, int _bufsize)
{
    inbuf = _inbuf;
    bufsize =  _bufsize;
    cursym = 0;
    currStr.num = 1;
    inbuf_markcurrstr();
}
