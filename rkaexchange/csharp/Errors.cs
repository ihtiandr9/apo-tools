using System;

namespace RkaExchange
{
    enum ErrorType
    {
        CmdRequired,
        UnknownCmd,
        NoExist,
        Create,
        Format,
        Pipe,
        Undef
    }

    static class Errors
    {
        public static void Throw(ErrorType type, string data = null)
        {
            Console.Error.Write("Error: ");
            switch (type)
            {
                case ErrorType.CmdRequired:
                    Console.Error.WriteLine("Command \"pack\" or \"unpack\" required");
                    Environment.Exit((int)type);
                    break;
                case ErrorType.UnknownCmd:
                    Console.Error.WriteLine("Unknown command: {0}", data);
                    Environment.Exit((int)type);
                    break;
                case ErrorType.Create:
                    Console.Error.WriteLine("Can't create output file: {0}", data);
                    Environment.Exit((int)type);
                    break;
                case ErrorType.Format:
                    Console.Error.WriteLine("Error of file format: {0}", data);
                    break;
                case ErrorType.NoExist:
                    Console.Error.WriteLine("File not exist: {0}", data);
                    Environment.Exit((int)type);
                    break;
                case ErrorType.Pipe:
                    Console.Error.WriteLine(data);
                    Environment.Exit((int)ErrorType.NoExist);
                    break;
                default:
                    Console.Error.WriteLine("Unknown error");
                    Environment.Exit(-1);
                    break;
            }
        }
    }
}
