using System;
using System.IO;

namespace Bkasm
{
    public static class Globals
    {
        public static BkasmStage bkasm_stage;
        public static string outFilename;

        public static void ExitNicely(eErrorType code)
        {
            Inbuf.Cleanup();
            Environment.Exit((int)code);
        }
    }

    class Program
    {
        static int Main(string[] args)
        {
            string infile = null;
            string outfile = null;

            if (args.Length >= 1 && (args[0] == "--help" || args[0] == "-h"))
            {
                Console.WriteLine("bkasm — i8080 assembler");
                Console.WriteLine("Usage: bkasm [infile] [outfile]");
                Console.WriteLine("  infile   source file (default: stdin)");
                Console.WriteLine("  outfile  output file (default: prog.bin in current dir)");
                Globals.ExitNicely(eErrorType.E_SUCCESS);
                return 0;
            }

            if (args.Length > 0)
                infile = args[0];
            if (args.Length > 1)
                outfile = args[1];

            byte[] buf;

            if (infile != null)
            {
                if (!File.Exists(infile))
                {
                    Console.Error.WriteLine("file not exist");
                    Globals.ExitNicely(eErrorType.E_INTERNALERROR);
                    return -1;
                }
                buf = File.ReadAllBytes(infile);
            }
            else
            {
                using (var ms = new MemoryStream())
                {
                    int b;
                    while ((b = Console.OpenStandardInput().ReadByte()) != -1)
                        ms.WriteByte((byte)b);
                    buf = ms.ToArray();
                }
            }

            Globals.outFilename = outfile;
            Console.WriteLine("program text size={0}", buf.Length);

            Inbuf.Init(buf, buf.Length);
            var lexer = new LexerBK();
            var parser = new ParserBK();
            parser.Parse(lexer);

            byte[] output = null;
            if (parser.error == eErrorType.E_SUCCESS)
                output = CodeGen.Link(parser.ast);
            Inbuf.Cleanup();

            if (output != null)
            {
                string fn = outfile ?? "prog.bin";
                try
                {
                    File.WriteAllBytes(fn, output);
                }
                catch (Exception ex)
                {
                    Console.Error.WriteLine("Error writing output file: {0}", ex.Message);
                    Globals.ExitNicely(eErrorType.E_LINKERERROR);
                    return -1;
                }
            }

            Globals.ExitNicely(eErrorType.E_SUCCESS);
            return 0;
        }
    }
}
