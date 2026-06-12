#ifndef H_INBUF_H
#define H_INBUF_H

#define INBUF_STRING_SIZE 512

STRUCT(InbufCurrentString)
{
    int num;
    unsigned char str[512];
};

#define MAX_INCLUDE_DEPTH 16

STRUCT(InbufFrame)
{
    const char* buf;
    int bufsize;
    int cursym;
    InbufCurrentString currStr;
};

unsigned char inbuf_next_char(void);
void inbuf_init(const char*, int size);
InbufCurrentString* inbuf_currstr();
int inbuf_push_file(const char *filename);
void inbuf_cleanup(void);

#endif


