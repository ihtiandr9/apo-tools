using System;
using System.IO;
using System.Text;

namespace RkaExchange
{
    class Edb
    {
        private static readonly string Koi7Table =
            " ▘▝▀▗▚▐▜ ★⬯↑⬮ ↣↓" +
            "▖▌▞▛▄▙▟█∼≈╋┃━↢✿▕" +
            " !\"#¤%&'()*+,-./" +
            "0123456789:;<=>?" +
            "@ABCDEFGHIJKLMNO" +
            "PQRSTUVWXYZ[\\]^_" +
            "ЮАБЦДЕФГХИЙКЛМНО" +
            "ПЯРСТУЖВЬЫЗШЭЩЧ▒";

        private static readonly string AsciiTable =
            " ▘▝▀▗▚▐▜ ★⬯↑⬮ ↣↓" +
            "▖▌▞▛▄▙▟█∼≈╋┃━↢✿▕" +
            " !\"#¤%&'()*+,-./" +
            "0123456789:;<=>?" +
            "@ABCDEFGHIJKLMNO" +
            "PQRSTUVWXYZ[\\]^_" +
            "abwgdevzijklmnop" +
            "rstufhc~{}.yx|`q";

        private static byte[] _asciiBytes;

        static Edb()
        {
            _asciiBytes = Encoding.UTF8.GetBytes(AsciiTable);
        }

        private static char GetUnicodeSymbol(byte chr)
        {
            return Koi7Table[chr];
        }

        private static byte CharCodeAt(byte chr)
        {
            if (chr < 128)
                return chr;
            return _asciiBytes[chr + 11];
        }

        public static void Pack(Stream input, Stream output)
        {
            int b;
            while ((b = input.ReadByte()) != -1)
            {
                byte ch = (byte)b;
                if (ch == 0xd0)
                    continue;
                byte res = CharCodeAt(ch);
                output.WriteByte(res);
            }
        }

        public static void Unpack(Stream input, Stream output)
        {
            Inbuf inbuf = new Inbuf(input);
            byte curSym = inbuf.NextChar();
            int totalSize = 0;
            bool textStarted = false;

            while (curSym != 0xff)
            {
                if (totalSize == 0 && curSym != 0xe6)
                {
                    Console.Error.WriteLine("unsupported format");
                    Environment.Exit(-1);
                }

                if (!textStarted && curSym == 0xe6)
                {
                    curSym = inbuf.NextChar();
                    totalSize++;
                    continue;
                }

                if (!textStarted && curSym == 0)
                {
                    while (curSym == 0)
                        curSym = inbuf.NextChar();
                    curSym = inbuf.NextChar();
                    curSym = inbuf.NextChar();
                    curSym = inbuf.NextChar();
                    textStarted = true;
                    continue;
                }

                if (curSym == 13)
                {
                    output.WriteByte(10);
                    curSym = inbuf.NextChar();
                    continue;
                }

                if (curSym < 32)
                {
                    output.WriteByte(10);
                }
                else
                {
                    char uniSym = GetUnicodeSymbol(curSym);
                    if (uniSym > 0x400)
                    {
                        byte[] utf8 = Encoding.UTF8.GetBytes(new[] { uniSym });
                        output.Write(utf8, 0, utf8.Length);
                    }
                    else
                    {
                        output.WriteByte((byte)uniSym);
                    }
                }

                curSym = inbuf.NextChar();
            }
        }
    }
}
