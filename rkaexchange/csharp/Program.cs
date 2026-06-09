using System;
using System.IO;

namespace RkaExchange
{
    class Program
    {
        static void Main(string[] args)
        {
            int cmd = 2;

            if (args.Length <= 0)
                Errors.Throw(ErrorType.CmdRequired);

            if (args[0] == "pack")
                cmd = 1;
            else if (args[0] == "unpack")
                cmd = 2;
            else
                Errors.Throw(ErrorType.UnknownCmd, args[0]);

            string inFile = args.Length > 1 ? args[1] : null;
            string outFile = args.Length > 2 ? args[2] : null;

            Stream fd_in = Console.OpenStandardInput();
            Stream fd_out = Console.OpenStandardOutput();
            bool ownInput = false;
            bool ownOutput = false;

            if (inFile != null)
            {
                if (!File.Exists(inFile))
                    Errors.Throw(ErrorType.NoExist, inFile);
                fd_in = File.OpenRead(inFile);
                ownInput = true;
            }

            if (outFile != null)
            {
                try
                {
                    fd_out = File.Create(outFile);
                    ownOutput = true;
                }
                catch
                {
                    Errors.Throw(ErrorType.Create, outFile);
                }
            }

            if (cmd == 2)
                Edb.Unpack(fd_in, fd_out);
            else if (cmd == 1)
                Edb.Pack(fd_in, fd_out);

            if (ownInput) fd_in.Close();
            if (ownOutput) fd_out.Close();
        }
    }
}
