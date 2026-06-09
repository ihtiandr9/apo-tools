using System;
using System.IO;

namespace RkaExchange
{
    class Inbuf
    {
        private byte[] buf = new byte[512];
        private int bufsize;
        private int cursym;
        private Stream stream;

        public InbufCurrentString Currstr { get; private set; }

        public Inbuf(Stream stream)
        {
            this.stream = stream;
            bufsize = stream.Read(buf, 0, 512);
            cursym = 0;
            Currstr = new InbufCurrentString { Num = 1 };
            MarkCurrstr();
        }

        public byte NextChar()
        {
            byte chr;
            if (cursym < bufsize)
                chr = buf[cursym++];
            else
            {
                bufsize = stream.Read(buf, 0, 512);
                cursym = 0;
                if (bufsize > 0)
                    chr = buf[cursym++];
                else
                    return 0xff;
            }
            if (chr == 10)
            {
                Currstr.Num++;
                MarkCurrstr();
            }
            return chr;
        }

        private void MarkCurrstr()
        {
            int len = Math.Min(buf.Length - cursym, bufsize - cursym);
            for (int i = 0; i < len; i++)
            {
                byte b = buf[cursym + i];
                Currstr.Str[i] = b == 10 ? (byte)0 : b;
            }
        }
    }

    class InbufCurrentString
    {
        public int Num { get; set; }
        public byte[] Str { get; } = new byte[512];
    }
}
