
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
#include <inbuf.h>

static unsigned char inbuf[512];

static InbufCurrentString currStr;
static int bufsize;
static int cursym;
static FILE *infile;

InbufCurrentString *inbuf_currstr()
{
    return (InbufCurrentString *) &currStr;
}

static void inbuf_markcurrstr()
{
    int i;
    for (i = 0; i + cursym < bufsize; i++)
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
        bufsize = fread(inbuf, 1, 512, infile);
        cursym = 0;
        if (bufsize)
            chr = inbuf[cursym++];
        else
            chr = 0xff;
    }
    if (chr == 10)
    {
        currStr.num++;
        inbuf_markcurrstr();
    }
    return chr;
}

void inbuf_init(FILE *_in_file)
{
    infile = _in_file;
    bufsize = fread(inbuf, 1, 512, infile);
    cursym = 0;
    currStr.num = 1;
    inbuf_markcurrstr();
}
