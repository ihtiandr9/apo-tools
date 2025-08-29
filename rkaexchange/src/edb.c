
#include <string.h>
#include <inbuf.h>
#include <fcntl.h>
#include <assert.h>
#ifndef WIN32
#ifndef _Windows
#ifndef __MSDOS__
#include <unistd.h>
#endif
#endif
#endif
#include <errors.h>
#include <errno.h>
#include <edb.h>

wchar_t getUnicodeSymbol(uint8_t chr)
{
    static const wchar_t *const c_apogeySymbols = // koi7
        L" ▘▝▀▗▚▐▜ ★⬯↑⬮ ↣↓"
        L"▖▌▞▛▄▙▟█∼≈╋┃━↢✿▕"
        L" !\"#¤%&'()*+,-./"
        "0123456789:;<=>?"
        L"@ABCDEFGHIJKLMNO"
        L"PQRSTUVWXYZ[\\]^_"
        L"ЮАБЦДЕФГХИЙКЛМНО"
        L"ПЯРСТУЖВЬЫЗШЭЩЧ▒";
    return c_apogeySymbols[chr];
}

char charCodeAt(uint8_t chr)
{
    static const unsigned char *const c_asciiSymbols =
        " ▘▝▀▗▚▐▜ ★⬯↑⬮ ↣↓"
        "▖▌▞▛▄▙▟█∼≈╋┃━↢✿▕"
        " !\"#¤%&'()*+,-./"
        "0123456789:;<=>?"
        "@ABCDEFGHIJKLMNO"
        "PQRSTUVWXYZ[\\]^_"
        "abwgdevzijklmnop"
        "rstufhc~{}.yx|`q";
    unsigned char res;
    if (chr < 128)
    {
        res = chr;
    }
    else
    {
        res = c_asciiSymbols[chr + 11];
    }
    return res;
}

static int encode(FILE *fd_in, FILE *fd_out)
{
    static unsigned char const utfRu = 0xd0;
    unsigned char ch, res;
    int readed;
    assert(fd_in > 0 && fd_out > 0);
    for (readed = fread(&ch, 1, 1, fd_in); readed > 0; readed = fread(&ch, 1, 1, fd_in))
    {
        assert(readed > 0);
        if (utfRu == ch)
        {
            continue;
        }
        res = charCodeAt(ch);
        fwrite(&res, 1, 1, fd_out);
    }
    return 0;
}

static int unpack(FILE *fd_in, FILE *fd_out)
{
    static char const utfRu = 0xd0;
    wchar_t uniSym;
    unsigned char outSym;
    unsigned char curSym;
	char textStarted = 0;
    int totalSize = 0;
    assert(fd_in > 0 && fd_out > 0);
    inbuf_init(fd_in);
    curSym = inbuf_next_char();
    while (0xff != curSym)
    {
        if (!totalSize && (curSym != 0xe6))
        {
            printf("unsupported format\n");
            exit(-1);
        }
        if (!textStarted && (curSym == 0xe6))
        {
            curSym = inbuf_next_char();
            totalSize++;
            continue;
        };
        if (!textStarted && (curSym == 0))
        {
            while (!curSym)
                curSym = inbuf_next_char();
            curSym = inbuf_next_char();
            curSym = inbuf_next_char();
            curSym = inbuf_next_char();
            textStarted = 1;
            continue;
        }
        if (curSym == 13)
        {
            curSym = 10;
            fwrite(&curSym, 1, 1, fd_out);
            curSym = inbuf_next_char();
            continue;
        }
        if (curSym < 32)
        {
            curSym = 10;
            fwrite(&curSym, 1, 1, fd_out);
        }
        else
        {
            uniSym = getUnicodeSymbol(curSym);
            outSym = uniSym;
            if (uniSym > 0x400)
            {
                outSym += 0x80;
                fwrite(&utfRu, 1, 1, fd_out);
                fwrite(&outSym, 1, 1, fd_out);
            }
            else
                fwrite(&uniSym, 1, 1, fd_out);
        }
        curSym = inbuf_next_char();
    }
    return 0;
}

static int pack(FILE *fd_in, FILE *fd_out)
{
    char buf[512];
    FILE *fd_tmp = fopen("./tmpfile", "w+");
    int readed;
    long f_pos;
    encode(fd_in, fd_tmp);
    f_pos = fseek(fd_tmp, 0, SEEK_SET);
    if(f_pos == -1)
        fprintf(stderr, "Error %d: %s", errno, strerror(errno));

    for(readed = fread(buf, 1, 512, fd_tmp); readed > 0; readed = fread(buf, 1, 512, fd_tmp))
    {
        fwrite(buf, readed, 1, fd_out);
    }
    if (readed == -1)
        fprintf(stderr,"Error %d: %s", errno, strerror(errno));
    fclose(fd_tmp);
    remove("./tmpfile");
    return 0;
}

static int edb_unpack(ptrEdb this)
{
    return unpack(this->fd_in, this->fd_out);
}

static int edb_pack(ptrEdb this)
{
    return pack(this->fd_in, this->fd_out);
}

void edb_init(FILE *fd_in, FILE *fd_out, ptrEdb pEdb)
{
    pEdb->fd_in = fd_in;
    pEdb->fd_out = fd_out;
    pEdb->unpack = edb_unpack;
    pEdb->pack = edb_pack;
}

ptrEdb edb_create(FILE *fd_in, FILE *fd_out)
{
    ptrEdb m_pEdb = malloc(sizeof(Edb));
    assert(m_pEdb);
    edb_init(fd_in, fd_out, m_pEdb);
    return m_pEdb;
}

