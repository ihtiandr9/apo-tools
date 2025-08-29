#include <stdio.h>
typedef struct InbufCurrentString InbufCurrentString;
#define INBUF_STRING_SIZE 512

struct InbufCurrentString
{
    int num;
    unsigned char str[512];
};

unsigned char inbuf_next_char(void);
void inbuf_init(const char*, int size);
InbufCurrentString* inbuf_currstr();
