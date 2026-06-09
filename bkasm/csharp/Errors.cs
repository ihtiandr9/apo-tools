using System;

namespace Bkasm
{
    public static class ErrorHandler
    {
        public static void ThrowError(eErrorType type, object data)
        {
            Console.Error.Write("Error: ");
            switch (type)
            {
                case eErrorType.E_UNKIDENT:
                    Console.Error.WriteLine("Unknown identifier: {0}", data);
                    break;
                case eErrorType.E_UNEXPSYM:
                    Console.Error.WriteLine("Unexpected symbol: \"{0}\" (may be in lowercase)", data);
                    break;
                case eErrorType.E_UNEXPTOKEN:
                    Console.Error.WriteLine("Unexpected token: {0}", data);
                    break;
                case eErrorType.E_UNKKEYWORD:
                    Console.Error.WriteLine("Unexpected keyword: {0}", data);
                    break;
                case eErrorType.E_SYNTAXERROR:
                    Console.Error.WriteLine("Syntax error in expression: {0}", data);
                    break;
                case eErrorType.E_LINKERERROR:
                    Console.Error.WriteLine("Linker error: {0}", data);
                    break;
                case eErrorType.E_INTERNALERROR:
                    Console.Error.WriteLine("Internal error: {0}", data);
                    break;
                default:
                    Console.Error.WriteLine("Unknown error");
                    Globals.ExitNicely(eErrorType.E_INTERNALERROR);
                    break;
            }
        }
    }
}
