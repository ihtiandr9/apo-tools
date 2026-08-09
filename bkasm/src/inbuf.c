
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#ifndef WIN32
#ifndef _Windows
#ifndef __MSDOS__
#include <unistd.h>
#endif
#endif
#endif
#include "bkasm.h"
#include "inbuf.h"

static InbufFrame frames[MAX_INCLUDE_DEPTH];
static int frame_count;

static const char* inbuf;
static int bufsize;
static int cursym;
static InbufCurrentString currStr;

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

static void inbuf_save_frame(int idx)
{
    frames[idx].buf = inbuf;
    frames[idx].bufsize = bufsize;
    frames[idx].cursym = cursym;
    frames[idx].currStr = currStr;
}

static void inbuf_restore_frame(int idx)
{
    inbuf = frames[idx].buf;
    bufsize = frames[idx].bufsize;
    cursym = frames[idx].cursym;
    currStr = frames[idx].currStr;
}

static void inbuf_pop(void)
{
    int idx = frame_count - 1;
    --frame_count;
    free((void*)frames[idx].buf);
    if (frame_count > 0)
        inbuf_restore_frame(frame_count - 1);
}

unsigned char inbuf_next_char()
{
    unsigned char chr;
    if (cursym < bufsize)
        chr = inbuf[cursym++];
    else if (frame_count > 1)
    {
        inbuf_pop();
        return inbuf_next_char();
    }
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
    frame_count = 1;
    inbuf = _inbuf;
    bufsize = _bufsize;
    cursym = 0;
    currStr.num = 1;
    inbuf_markcurrstr();
    inbuf_save_frame(0);
}

int inbuf_push_file(const char *filename)
{
    FILE *f;
    long sz;
    char *buf;

    if (frame_count >= MAX_INCLUDE_DEPTH)
        return -1;

    inbuf_save_frame(frame_count - 1);

    f = fopen(filename, "r");
    if (!f)
        return -1;

    fseek(f, 0, SEEK_END);
    sz = ftell(f);
    if (sz < 0)
    {
        fclose(f);
        return -1;
    }
    rewind(f);

    buf = malloc(sz + 1);
    if (!buf)
    {
        fclose(f);
        return -1;
    }

    if (fread(buf, 1, sz, f) != (size_t)sz)
    {
        free(buf);
        fclose(f);
        return -1;
    }
    buf[sz] = '\0';
    fclose(f);

    inbuf = buf;
    bufsize = sz;
    cursym = 0;
    currStr.num = 1;
    inbuf_markcurrstr();

    inbuf_save_frame(frame_count);
    ++frame_count;
    return 0;
}

void inbuf_cleanup(void)
{
    while (frame_count > 1)
    {
        int idx = frame_count - 1;
        --frame_count;
        free((void*)frames[idx].buf);
    }
    frame_count = 0;
}
