
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

static unsigned char inbuf[512];
static int bufsize;
static int cursym;
static int fd;

unsigned char inbuf_next_next_char()
{
	if(cursym < bufsize)
		return inbuf[cursym++];
	bufsize = read(fd, inbuf, 512);
	cursym = 0;
	if(bufsize)
		return inbuf[cursym++];
	else
		return 0xff;
}

void inbuf_init( int _fd)
{
	fd = _fd;
	bufsize = read(fd, inbuf, 512);
	cursym = 0;
}
