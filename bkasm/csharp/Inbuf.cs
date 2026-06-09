using System;
using System.IO;

namespace Bkasm
{
    public class InbufCurrentString
    {
        public int num;
        public char[] str = new char[Constants.INBUF_STRING_SIZE];

        public InbufCurrentString()
        {
            num = 1;
        }
    }

    public class InbufFrame
    {
        public byte[] buf;
        public int bufsize;
        public int cursym;
        public InbufCurrentString currStr;
    }

    public static class Inbuf
    {
        public const int MAX_INCLUDE_DEPTH = 16;

        private static InbufFrame[] frames = new InbufFrame[MAX_INCLUDE_DEPTH];
        private static int frameCount;
        private static byte[] inbuf;
        private static int bufsize;
        private static int cursym;
        private static InbufCurrentString currStr = new InbufCurrentString();

        public static InbufCurrentString CurrStr()
        {
            return currStr;
        }

        private static void MarkCurrStr()
        {
            for (int i = 0; i + cursym < bufsize && i < Constants.INBUF_STRING_SIZE; i++)
            {
                currStr.str[i] = (char)inbuf[cursym + i];
                if (currStr.str[i] == '\n')
                    currStr.str[i] = '\0';
            }
        }

        private static void SaveFrame(int idx)
        {
            frames[idx] = new InbufFrame();
            frames[idx].buf = inbuf;
            frames[idx].bufsize = bufsize;
            frames[idx].cursym = cursym;
            frames[idx].currStr = new InbufCurrentString();
            frames[idx].currStr.num = currStr.num;
            Array.Copy(currStr.str, frames[idx].currStr.str, Constants.INBUF_STRING_SIZE);
        }

        private static void RestoreFrame(int idx)
        {
            inbuf = frames[idx].buf;
            bufsize = frames[idx].bufsize;
            cursym = frames[idx].cursym;
            currStr.num = frames[idx].currStr.num;
            Array.Copy(frames[idx].currStr.str, currStr.str, Constants.INBUF_STRING_SIZE);
        }

        private static void Pop()
        {
            int idx = frameCount - 1;
            --frameCount;
            frames[idx] = null;
            if (frameCount > 0)
                RestoreFrame(frameCount - 1);
        }

        public static byte NextChar()
        {
            byte chr;
            if (cursym < bufsize)
                chr = inbuf[cursym++];
            else if (frameCount > 1)
            {
                Pop();
                return NextChar();
            }
            else
                chr = Constants.EOF_SYM;

            if (chr == '\n')
            {
                currStr.num++;
                MarkCurrStr();
            }
            return chr;
        }

        public static void Init(byte[] buf, int size)
        {
            frameCount = 1;
            inbuf = buf;
            bufsize = size;
            cursym = 0;
            currStr.num = 1;
            MarkCurrStr();
            SaveFrame(0);
        }

        public static int PushFile(string filename)
        {
            if (frameCount >= MAX_INCLUDE_DEPTH)
                return -1;

            SaveFrame(frameCount - 1);

            byte[] buf;
            try
            {
                buf = File.ReadAllBytes(filename);
            }
            catch
            {
                return -1;
            }

            inbuf = buf;
            bufsize = buf.Length;
            cursym = 0;
            currStr.num = 1;
            MarkCurrStr();

            SaveFrame(frameCount);
            ++frameCount;
            return 0;
        }

        public static void Cleanup()
        {
            while (frameCount > 1)
            {
                int idx = frameCount - 1;
                --frameCount;
                frames[idx] = null;
            }
            frameCount = 0;
        }
    }
}
