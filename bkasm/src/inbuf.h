#ifndef H_INBUF_H
#define H_INBUF_H

#define INBUF_STRING_SIZE 512

STRUCT(InbufCurrentString)
{
    int num;
    unsigned char str[512];
};

unsigned char inbuf_next_char(void);
void inbuf_init(const char*, int size);
InbufCurrentString* inbuf_currstr();

#endif


