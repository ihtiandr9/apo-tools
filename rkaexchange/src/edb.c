
#include <edb.h>
#include <inbuf.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

wchar_t getUnicodeSymbol(uint8_t chr)
{
    static const wchar_t *const c_apogeySymbols = // koi7
        L" ▘▝▀▗▚▐▜ ★⬯↑⬮ ↣↓"
        L"▖▌▞▛▄▙▟█∼≈╋┃━↢✿▕"
        L" !\"#¤%&'()*+,-./0123456789:;<=>?"
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

static int encode(int fd_in, int fd_out)
{
    unsigned char ch, res;
    assert(fd_in > 0 && fd_out > 0);
    for (int readed = read(fd_in, &ch, 1); readed > 0; readed = read(fd_in, &ch, 1))
    {
        assert(readed > 0);
        if (0xd0 == ch)
        {
            continue;
        }
        res = charCodeAt(ch);
        write(fd_out, &res, 1);
    }
    return 0;
}

static int unpack(int fd_in, int fd_out)
{
    assert(fd_in > 0 && fd_out > 0);
    static char const utfRu = 0xd0;
    wchar_t uniSym;
    unsigned char outSym;
    char textStarted = 0;
    int totalSize = 0;
    inbufInit(fd_in);
    unsigned char curSym = inbufNextChar();
    while (0xff != curSym)
    {
        if (!totalSize && (curSym != 0xe6))
        {
            printf("unsupported format\n");
            exit(-1);
        }
        if (!textStarted && (curSym == 0xe6))
        {
            curSym = inbufNextChar();
            totalSize++;
            continue;
        };
        if (!textStarted && (curSym == 0))
        {
            while (!curSym)
                curSym = inbufNextChar();
            curSym = inbufNextChar();
            curSym = inbufNextChar();
            curSym = inbufNextChar();
            textStarted = 1;
            continue;
        }
        if (curSym == 13)
        {
            curSym = 10;
            write(fd_out, &curSym, 1);
            curSym = inbufNextChar();
            continue;
        }
        if (curSym < 32)
        {
            curSym = 10;
            write(fd_out, &curSym, 1);
        }
        else
        {
            uniSym = getUnicodeSymbol(curSym);
            outSym = uniSym;
            if (uniSym > 0x400)
            {
                outSym += 0x80;
                write(fd_out, &utfRu, 1);
                write(fd_out, &outSym, 1);
            }
            else
                write(fd_out, &uniSym, 1);
        }
        curSym = inbufNextChar();
    }
    return 0;
}

static int pack(int fd_in, int fd_out)
{
    int fd_tmp = open("./tmpfile", O_CREAT | O_RDWR, 0655);
    encode(fd_in, fd_tmp);
    lseek(fd_tmp, 0, SEEK_SET);
    char buf[512];
    for(int readed = read(fd_tmp, buf, 512); readed; readed = read(fd_tmp, buf, 512))
    {
        write(fd_out, buf, readed);
    }
    return 0;
}

static inline int edb_unpack(ptrEdb this)
{
    return unpack(this->fd_in, this->fd_out);
}

static inline int edb_pack(ptrEdb this)
{
    return pack(this->fd_in, this->fd_out);
}

ptrEdb edb_create(int fd_in, int fd_out)
{
    ptrEdb m_pEdb = malloc(sizeof(Edb));
    if (m_pEdb)
    {
        m_pEdb->fd_in = fd_in;
        m_pEdb->fd_out = fd_out;
        m_pEdb->unpack = edb_unpack;
        m_pEdb->pack = edb_pack;
    }
    return m_pEdb;
}

