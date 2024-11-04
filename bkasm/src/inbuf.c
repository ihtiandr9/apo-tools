
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#ifndef WIN32
#include <unistd.h>
#endif

static unsigned char inbuf[512];
static unsigned char currstr[512];
static int bufsize;
static int cursym;
static int fd;

char* inbuf_currstr()
{
    return (char *)currstr;
}

static void inbuf_markcurrstr()
{
    int i;
    for (i = 0; i + cursym < bufsize; i++)
    {
        currstr[i] = inbuf[cursym + i];
        if (currstr[i] == 10)
            currstr[i] = 0;
    }
}

unsigned char inbuf_next_char()
{
    unsigned char chr;
    if (cursym < bufsize)
        chr = inbuf[cursym++];
    else
    {
        bufsize = read(fd, inbuf, 512);
        cursym = 0;
        if (bufsize)
            chr = inbuf[cursym++];
        else
            chr = 0xff;
    }
    if (chr == 10)
        inbuf_markcurrstr();
    return chr;
}

void inbuf_init(int _fd)
{
    fd = _fd;
    bufsize = read(fd, inbuf, 512);
    cursym = 0;
    currstr[0] = 0;
}
