
#include <edb.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

wchar_t getUnicodeSymbol(uint8_t chr)
{
        static wchar_t *c_apogeySymbols = // koi7
            L" ▘▝▀▗▚▐▜ ★⬯↑⬮ ↣↓▖▌▞▛▄▙▟█∼≈╋┃━↢✿▕"
            L" !\"#¤%&'()*+,-./0123456789:;<=>?"
            L"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
            L"ЮАБЦДЕФГХИЙКЛМНОПЯРСТУЖВЬЫЗШЭЩЧ▒";
        return c_apogeySymbols[chr];
}

char charCodeAt(uint8_t chr)
{
        static const unsigned char const *c_asciiSymbols =
            "abwgdegzijklmnoprstufhc~{}xyx|`q";
        unsigned char res;
        if (chr < 128)
        {
                res = chr;
        }
        else
        {
                res = c_asciiSymbols[chr - 0x90];
        }
        return res;
}

static int pack(int fd_in, int fd_out)
{
        printf("In progress\n");
        unsigned char ch, res;
        assert(fd_in > 0 && fd_out > 0);
        for (int readed = read(fd_in, &ch, 1); readed > 0; readed = read(fd_in, &ch, 1))
        {
                assert(readed > 0);
                //    unsigned char *curSym = buf;
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
        unsigned char buf[256];
        wchar_t uniSym;
        unsigned char outSym;
        char textStarted;
        int totalSize = 0;
        for (int readed = read(fd_in, buf, 254); readed > 0; readed = read(fd_in, buf, 254))
        {
                assert(readed > 0);
                unsigned char *curSym = buf;
                while ((curSym - buf < readed) && 0xff != (*curSym))
                {
                        if (!totalSize && (*curSym != 0xe6))
                        {
                                printf("unsupported format\n");
                                exit(-1);
                        }
                        if (!textStarted && (*curSym == 0xe6))
                        {
                                curSym += 16;
                                totalSize += 16;
                                while (*curSym == 0 && (curSym - buf < readed))
                                {
                                        curSym++;
                                        totalSize++;
                                }
                                continue;
                        };
                        if (!textStarted && (*curSym == 0))
                        {
                                while (*curSym == 0 && (curSym - buf < readed))
                                        curSym++;
                                if (curSym - buf == readed)
                                        continue;
                                curSym += 3;
                                textStarted = 1;
                                continue;
                        }
                        if (*curSym == 13)
                        {
                                *curSym = 10;
                                write(fd_out, curSym, 1);
                                curSym++;
                                continue;
                        }
                        if (*curSym < 32)
                        {
                                *curSym = 10;
                                write(fd_out, curSym, 1);
                        }
                        else
                        {
                                uniSym = getUnicodeSymbol(*curSym);
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
                        curSym++;
                }
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