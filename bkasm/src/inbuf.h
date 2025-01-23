
#include <stdio.h>
typedef struct InbufCurrentString InbufCurrentString;
struct InbufCurrentString
{
    int num;
    unsigned char str[512];
};

unsigned char inbuf_next_char(void);
void inbuf_init( FILE* in_file);
InbufCurrentString* inbuf_currstr();
